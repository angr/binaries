/* The loop shape of coreutils cat() (src/cat.c), reduced until angr master stops failing to structure it.
 *
 *   gcc -O2 -fno-inline -o cat_shape cat_shape.c && strip -o cat_shape_stripped cat_shape
 *
 * Decompiling copy() (0x401280 with angr's 0x400000 base for the stripped PIE; check with `nm cat_shape`) on
 * angr master ea12bae6f reports Decompiler.structuring_failures == [0x401280] and drops most of the body:
 *
 *   python decompile.py cat_shape_stripped 401280
 *
 * The helpers carry noipa: once gcc can see that write_error() never returns, the flush path stops being a loop
 * exit and the shape no longer triggers the problem.
 *
 * What matters: the outer while (1) starts with a do-while that refills the buffer; gcc lays the loop out so that
 * it is entered twice from the same block, once at the flush check and once through the do-while's tail test, so
 * the region identifier's head does not dominate its latch (an irreducible loop) and the slice it falls back to
 * swallows the enclosing loop. Every one of the following is needed to keep the failure on master: the flush
 * check at the top of the do-while, the two separate early returns that each store newlines2, the line-number
 * call after the do-while, and the two-way character loop with the tab branch.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int input_desc = 0;
int newlines2 = -1;

__attribute__((noinline, noipa)) void write_error(void) { _exit(1); }
__attribute__((noinline, noipa)) void write_pending(char *outbuf, char **bpout)
{
    if (*bpout > outbuf) {
        if (write(1, outbuf, *bpout - outbuf) < 0)
            write_error();
        *bpout = outbuf;
    }
}
__attribute__((noinline, noipa)) char *line_number(char *bpout)
{
    static int n;
    return bpout + sprintf(bpout, "%6d\t", ++n);
}

bool copy(char *inbuf, size_t insize, char *outbuf, size_t outsize, bool number, bool show_tabs)
{
    unsigned char ch;
    int newlines = newlines2;
    char *eob = inbuf;
    char *bpin = eob + 1;
    char *bpout = outbuf;

    while (1) {
        do {
            if (outbuf + outsize <= bpout) {
                if (write(1, outbuf, bpout - outbuf) < 0)
                    write_error();
                bpout = outbuf;
            }

            if (bpin > eob) {
                write_pending(outbuf, &bpout);
                ssize_t n_read = read(input_desc, inbuf, insize);
                if (n_read < 0) {
                    newlines2 = newlines;
                    return false;
                }
                if (n_read == 0) {
                    newlines2 = newlines;
                    return true;
                }
                bpin = inbuf;
                eob = bpin + n_read;
                *eob = '\n';
            } else {
                *bpout++ = '\n';
            }
            ch = *bpin++;
        } while (ch == '\n');

        if (newlines >= 0 && number)
            bpout = line_number(bpout);

        while (1) {
            if (ch == '\t' && show_tabs) {
                *bpout++ = '^';
                *bpout++ = ch + 64;
            } else if (ch != '\n') {
                *bpout++ = ch;
            } else {
                newlines = -1;
                break;
            }
            ch = *bpin++;
        }
    }
}

int main(int argc, char **argv)
{
    static char inbuf[4096 + 1], outbuf[8192];
    bool number = argc > 1 && strchr(argv[1], 'n');
    bool tabs = argc > 1 && strchr(argv[1], 'T');
    return copy(inbuf, 4096, outbuf, 4096, number, tabs) ? 0 : 1;
}

/*
 * Handlers reached only through a table of function pointers, dispatched with a tail call.
 *
 * decode() and every handler end in `return table[byte](d)`, which GCC -O2 compiles to an indirect jump through
 * the table. The handlers are ordinary functions that return; nothing calls them directly. This is how decoders
 * such as Intel's libipt dispatch on an instruction prefix.
 */
#include <stddef.h>

struct decoder {
    const unsigned char *buf;
    size_t len;
    size_t pos;
    unsigned flags;
};

typedef int (*handler_t)(struct decoder *d);

static int op_done(struct decoder *d) __attribute__((noinline));
static int op_lock(struct decoder *d) __attribute__((noinline));
static int op_rep(struct decoder *d) __attribute__((noinline));
static int op_seg(struct decoder *d) __attribute__((noinline));
static int op_size(struct decoder *d) __attribute__((noinline));
static int op_rex(struct decoder *d) __attribute__((noinline));
static int op_bad(struct decoder *d) __attribute__((noinline));
static int op_vex(struct decoder *d) __attribute__((noinline));

static handler_t const table[8] = { op_done, op_lock, op_rep, op_seg, op_size, op_rex, op_bad, op_vex };

static int next(struct decoder *d)
{
    if (d->pos >= d->len)
        return -1;
    return table[d->buf[d->pos++] & 7](d);
}

static int op_done(struct decoder *d) { return (int)(d->pos + d->flags); }
static int op_lock(struct decoder *d) { d->flags |= 1; return next(d); }
static int op_rep(struct decoder *d) { d->flags |= 2; return next(d); }
static int op_seg(struct decoder *d) { d->flags = (d->flags & ~0xf0u) | ((unsigned)d->buf[d->pos - 1] << 4); return next(d); }
static int op_size(struct decoder *d) { d->flags ^= 0x100; return next(d); }
static int op_rex(struct decoder *d) { if (d->flags & 0x200) return -2; d->flags |= 0x200; return next(d); }
static int op_bad(struct decoder *d) { return -3 - (int)d->pos; }
static int op_vex(struct decoder *d) { if (d->len - d->pos < 2) return -4; d->pos += 2; return (int)(d->flags | 0x400); }

int decode(struct decoder *d) __attribute__((noinline));

int decode(struct decoder *d)
{
    d->pos = 0;
    d->flags = 0;
    return next(d);
}

int main(int argc, char **argv)
{
    struct decoder d = { (const unsigned char *)argv[0], (size_t)argc, 0, 0 };
    return decode(&d);
}

/*
 * A tentative definition too large for the small code model.
 *
 * Under -mcmodel=medium, gcc puts a common symbol whose size exceeds
 * -mlarge-data-threshold in the "large common" area rather than in .bss, and
 * gives it st_shndx = SHN_X86_64_LCOMMON (0xff02). That is a processor-specific
 * reserved index, not a section header table index, so a loader that uses
 * st_shndx to subscript the section list walks off the end of it.
 *
 * Built for cle's ELF symbol tests. See build.sh.
 */

char big_buffer[1 << 21];

int use_big_buffer(int i)
{
    return big_buffer[i];
}

/*
 * A PowerPC64 ELFv1 relocatable object that does map something: one function,
 * with the .opd function descriptor and the .text it points at both landing at
 * the start of the loaded image. It is ET_REL, so its e_entry is zero like
 * every other one, and a loader that follows e_entry as a descriptor pointer
 * reads this object's own first instructions instead of failing.
 */

extern int puts(const char *s);

int greet(void)
{
    return puts("hello");
}

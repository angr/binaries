/*
 * A PowerPC64 ELFv1 relocatable object with nothing to map: a translation unit
 * that declares an import and defines nothing, of which any real build produces
 * plenty. It is ET_REL, so its e_entry is zero, and no section carries
 * SHF_ALLOC content, so a loader that follows e_entry as an ELFv1 function
 * descriptor pointer has nowhere to read from at all.
 */

extern int puts(const char *s);

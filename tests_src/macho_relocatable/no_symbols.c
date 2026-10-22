/* Fixture source for Mach-O MH_OBJECTs compiled from a translation unit with
 * no code.
 *
 * No code, on purpose: clang writes no LC_SYMTAB for an x86_64 object with no
 * symbols to list. -g adds the four __DWARF accelerator tables; without it the
 * object is 208 bytes holding the same empty __text. Built for arm64 without
 * -g, the object maps nothing at all. Nothing in this file reaches the output,
 * and a zero-byte source compiles to the same bytes.
 *
 * Build (produces tests/x86_64/relocatable_object_no_symtab.macho and
 * tests/aarch64/relocatable_object_empty.macho):
 *     ./build.sh
 */

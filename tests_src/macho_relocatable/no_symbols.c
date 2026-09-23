/* Fixture source for a Mach-O MH_OBJECT that carries no symbol table.
 *
 * No code, on purpose: clang writes no LC_SYMTAB for an x86_64 object with no
 * symbols to list. -g adds the four __DWARF accelerator tables; without it the
 * object is 208 bytes holding the same empty __text. Nothing in this file
 * reaches the output, and a zero-byte source compiles to the same bytes.
 *
 * Build (produces tests/x86_64/relocatable_object_no_symtab.macho):
 *     ./build.sh
 */

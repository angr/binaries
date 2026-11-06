// A DLL that carries an export directory and a COFF symbol table at once. cle reads
// the symbol table to type a PE's exports, and no other PE under tests/ carries both,
// so that typing has nothing to be asserted against. This one exports a function, a
// data object and a forwarder; the table types the first two, and cle types a
// forwarded export as a function without consulting the table.
//
// coff_export_types.def names the three exports. C has no syntax for a forwarder.
//
// Built with x86_64-w64-mingw32-gcc 15.3.0, binutils 2.46, mingw-w64 14.0.0:
//
//     x86_64-w64-mingw32-gcc -shared -Wl,--no-insert-timestamp \
//         -Wl,--image-base,0x180000000 -o coff_export_types.dll \
//         coff_export_types.c coff_export_types.def
//
// That gcc is configured with the mcf threading model, so the link also needs -L
// pointing at its libmcfgthread.

int exported_counter = 7;

int exported_function(int value)
{
    return value + exported_counter;
}

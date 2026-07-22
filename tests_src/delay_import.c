/*
 * Minimal Windows PE with a delay-load import, for testing CLE's delay-import
 * binding (cle/backends/pe/pe.py: PE._handle_delay_imports).
 *
 * user32.dll is delay-loaded: its descriptor lands in the delay-import data
 * directory (entry 13) instead of the normal import directory, and its IAT
 * slot points at a __delayLoadHelper2 binding thunk until first use.
 *
 * GNU ld does not emit the delay-import directory, so this must be built with
 * lld and a short-format import library (produced by llvm-dlltool). See
 * tests_src/delay_import.build.sh for the exact recipe.
 */
#include <windows.h>

int main(void)
{
    /* delay-loaded call: resolves through the delay IAT slot */
    MessageBoxA(NULL, "delay-import test", "test", MB_OK);
    return 0;
}

// Used to check if cle respects RUNPATH in binaries.
// compile with:
// $ gcc -O0 -o ../tests/x86_64/runpath -Wl,-rpath,'$ORIGIN/lib' runpath.c
int main(void) {}

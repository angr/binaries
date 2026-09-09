/*
 * dummy_main.c -- Provides main() and g_sink definition for linking.
 *
 * Each source file declares ``extern volatile int g_sink;`` via
 * harness_common.h.  This file provides the actual definition and a
 * trivial main so the linker produces a valid executable.
 */

volatile int g_sink;

int main(void) {
    return 0;
}

/* Fake Swift binary source.
 * Compiled with gcc, then post-processed to add Swift-like symbols and
 * sections so the language detector can be tested without a Swift compiler.
 */
#include <stdio.h>

int main(void) {
    printf("hello from fake swift\n");
    return 0;
}

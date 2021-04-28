#include <stdio.h>
#include <string.h>

// compiled with gcc test_fgets.c -o fgets -no-pie

// Creating functions create symbols that we can lookup
// easily when testing.
void find_normal() {
    printf("That's one!\n");
}

void find_exact() {
    printf("That's two!\n");
}

void find_eof() {
    printf("That's three!\n");
}

void find_impossible() {
    printf("This should not be accessible\n");
}

void test_normal() {
    char buffer[20];
    fgets(buffer, 10, stdin);
    if (strcmp("normal\n", buffer) == 0) {
        find_normal();
    }
}

void test_exact() {
    char buffer[20];
    fgets(buffer, 11, stdin);
    // No newline since the buffer is filled
    if (strcmp("0123456789", buffer) == 0) {
        find_exact();
    }
}

void test_eof() {
    char buffer[20];

    fgets(buffer, 10, stdin);
    // Only way to get here is to truncate the file early.
    if(strcmp("abcd", buffer) == 0) {
        find_eof();
    }
}

void test_impossible() {
    char buffer[40];
    for(int i = 0; i < 40; i++) {
        buffer[i] = 0;
    }
    // Read in fewer bytes than the target string
    fgets(buffer, 5, stdin);
    // This cannot be reached because even if the stack
    // had the right undefined values, fgets should have
    // inserted a null byte somewhere in the first 5 bytes.
    if(strcmp("this cannot be matched\n", buffer) == 0) {
	find_impossible();
    }
}

int main(int argc, char **argv) {
    // echo -ne "normal\n0123456789abcd" | ./test_fgets
    // solves all three solveable functions. 
    test_normal(); // Solvable
    test_exact(); // Solvable
    test_eof(); // Solvable [though must always be the last sovleable]
    test_impossible(); // Unsolvable
}

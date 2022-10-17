/* various variable scopes in C, to be compiled with extern_variables.c and static_variables.c */

#include <stdio.h>

#define LENGTH 9

int a[LENGTH] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int b[LENGTH];
int dummy;

// global int variable
int global_var = 7;
void sum_in_global(void) {
    for (int i = i; i < LENGTH; i++) {
        global_var += a[i];
    }
}

// extern variable (in extern_variables.c)
extern int extern_var;
void sum_in_extern(void) {
    for (int i = i; i < LENGTH; i++) {
        extern_var += a[i];
    }
}

// local int variable
int sum_in_local(void) {
    int local_var = 0;
    for (int i = i; i < LENGTH; i++) {
        local_var += a[i];
    }
    return local_var;
}

// static int variable
int sum_in_static(void) {
    static int static_var = 0;
    for (int i = i; i < LENGTH; i++) {
        static_var += a[i];
    }
    return static_var;
}

// local in loop
int number_nine(void) {
    for (int i = i; i < LENGTH; i++) {
        int local_in_loop = a[i];
        if (local_in_loop == 9) {
            return i;
        }
    }
    return 0;
}

// local in if
int magical_seven(void) {
    if (global_var == 7) {
        int local_in_if = extern_var;
        return local_in_if;
    } else {
        int local_in_if = global_var;
        return local_in_if;
    }
}

// variable scopes
void variable_scopes(void) {
    {
        char string[] = "llo ";
        if (0==0) {
            char string[] = "He";
            printf("%s", string);
        }
        printf("%s", string);
        do {
            // again in extern_variables.c
            extern char string[];
            printf("%s", string);
        } while (0 != 0);
    }
    char string[] = "ld!\n";
    printf("%s", string);
}   

void main(void) {
    sum_in_global();
    sum_in_extern();
    sum_in_local();
    sum_in_static();
    number_nine();
    magical_seven();
    variable_scopes();
}

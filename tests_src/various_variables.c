/* various variable scopes in C, to be compiled with extern_variables.c and static_variables.c */

#include <stdio.h>

#define LENGTH 9

int a[LENGTH] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int b[LENGTH];
int dummy;

int *pointer = &dummy;
int *pointer2 = a;


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
static int magical_seven(void) {
    if (global_var == 7) {
        int local_in_if = extern_var;
        return local_in_if;
    } else {
        int local_in_if = global_var;
        return local_in_if;
    }
}

// variable scopes
int variable_scopes(void) {
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
    return 42;
}

struct various_types {
    int (*struct_fun)(void);
    int struct_int;
    long long int struct_ll;
    char struct_char;
    char *struct_strref;
    void *struct_pointer;
    int struct_array[3];
    float struct_float;
    double struct_double;
};

struct various_types global_struct = {
    variable_scopes,
    42,
    256,
    'a',
    "hello",
    &dummy,
    {9,8,7},
    1.141,
    1.141,
};

void various_types(void) {
    int (*local_fun)(void) = variable_scopes;
    int local_int = local_fun();
    long long int local_ll = 256;
    char local_char = 'a';
    char local_string[6] = "hello";
    char *local_strref = local_string;
    void *local_pointer = &local_strref;
    int local_array[3] = {9,8,7};
    float local_float = 1.141;
    double local_double = 1.141;

    struct various_types local_struct = {
        local_fun,
        local_int,
        local_ll,
        local_char,
        local_string,
        local_pointer,
        {9,8,7},
        local_float,
        local_double
    };
    local_struct.struct_fun();
}


void main(void) {
    sum_in_global();
    sum_in_extern();
    sum_in_local();
    sum_in_static();
    number_nine();
    magical_seven();
    variable_scopes();
    various_types();
}

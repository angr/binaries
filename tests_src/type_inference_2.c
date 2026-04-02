/*
 * calculator.c
 *
 * A simple calculator implemented in C using two structs:
 *   - Operand  : holds a numeric value and a label
 *   - Operation: holds two Operands and an operator character
 *
 * Functions:
 *   1. make_operand    – construct an Operand
 *   2. make_operation  – construct an Operation from two Operands
 *   3. evaluate        – compute the result of an Operation
 *   4. print_result    – pretty-print an Operation and its result
 *   5. run_calculator  – driver that exercises the four functions above
 */

#include <stdio.h>

/* ── Structs ─────────────────────────────────────────────────────────────── */

typedef struct {
    double value;       /* the numeric value of this operand          */
    char   label[32];   /* human-readable name, e.g. "a", "b", "3.5" */
} Operand;

typedef struct {
    Operand left;       /* left-hand operand                          */
    Operand right;      /* right-hand operand                         */
    char    op;         /* operator: '+', '-', '*', '/'               */
} Operation;

/* ── Function 1: make_operand ────────────────────────────────────────────── */

/*
 * Construct and return an Operand with the given value.
 * The label is generated automatically from the value.
 */
Operand make_operand(double value)
{
    Operand o;
    o.value = value;
    snprintf(o.label, sizeof(o.label), "%.6g", value);
    return o;
}

/* ── Function 2: make_operation ─────────────────────────────────────────── */

/*
 * Combine two Operands and an operator into an Operation struct.
 * The Operands are passed by value (local struct copies).
 */
Operation make_operation(Operand left, Operand right, char op)
{
    Operation expr;
    expr.left  = left;
    expr.right = right;
    expr.op    = op;
    return expr;
}

/* ── Function 3: evaluate ───────────────────────────────────────────────── */

/*
 * Compute the numeric result of an Operation.
 * Returns 1 on success, 0 on error (e.g. division by zero).
 * The result is written through the pointer `out`.
 *
 * The Operation struct is passed by pointer to avoid copying,
 * but it is treated as read-only inside this function.
 */
int evaluate(const Operation *expr, double *out)
{
    double l = expr->left.value;
    double r = expr->right.value;

    switch (expr->op) {
        case '+': *out = l + r;         return 1;
        case '-': *out = l - r;         return 1;
        case '*': *out = l * r;         return 1;
        case '/':
            if (r == 0.0) {
                fprintf(stderr, "Error: division by zero.\n");
                return 0;
            }
            *out = l / r;
            return 1;
        default:
            fprintf(stderr, "Error: unknown operator '%c'.\n", expr->op);
            return 0;
    }
}

/* ── Function 4: print_result ───────────────────────────────────────────── */

/*
 * Pretty-print an Operation together with its pre-computed result.
 * Both the Operation and the result value are passed in.
 */
void print_result(const Operation *expr, double result)
{
    printf("  %s %c %s  =  %.6g\n",
           expr->left.label,
           expr->op,
           expr->right.label,
           result);
}

/* ── Function 5: run_calculator ─────────────────────────────────────────── */

/*
 * Driver function.  Creates local Operand and Operation variables,
 * then passes them into the helper functions above.
 */
void run_calculator(void)
{
    /* Define a table of test cases: { left_val, right_val, operator } */
    struct {
        double left_val;
        double right_val;
        char   op;
    } tests[] = {
        { 12.0,   4.0,  '+' },
        { 12.0,   4.0,  '-' },
        { 12.0,   4.0,  '*' },
        { 12.0,   4.0,  '/' },
        { 7.5,    2.5,  '+' },
        { 100.0,  33.0, '-' },
        { 6.0,    7.0,  '*' },
        { 22.0,   0.0,  '/' },   /* division-by-zero demo */
    };

    int n = sizeof(tests) / sizeof(tests[0]);

    printf("=== Calculator Results ===\n");

    for (int i = 0; i < n; i++) {
        /* Build local Operand structs and pass them into make_operation */
        Operand  a    = make_operand(tests[i].left_val);
        Operand  b    = make_operand(tests[i].right_val);
        Operation expr = make_operation(a, b, tests[i].op);

        double result = 0.0;
        if (evaluate(&expr, &result)) {
            print_result(&expr, result);
        }
    }

    printf("=========================\n");
}

/* ── main ───────────────────────────────────────────────────────────────── */

int main(void)
{
    run_calculator();
    return 0;
}

/* Test C file for enum support in angr decompilation */

/* Simple enum with implicit values */
enum Color {
    RED,
    GREEN,
    BLUE
};

/* Enum with explicit values */
enum Status {
    OK = 0,
    ERROR = -1,
    PENDING = 100,
    UNKNOWN = 999
};

/* Enum with mixed implicit/explicit values */
enum Mixed {
    A,          /* 0 */
    B = 10,     /* 10 */
    C,          /* 11 */
    D = 20      /* 20 */
};

/* Anonymous enum */
enum {
    FLAG_NONE = 0,
    FLAG_READ = 1,
    FLAG_WRITE = 2,
    FLAG_EXEC = 4
};

/* Function using enums */
int color_to_rgb(enum Color c) {
    switch (c) {
        case RED:   return 0xFF0000;
        case GREEN: return 0x00FF00;
        case BLUE:  return 0x0000FF;
        default:    return 0x000000;
    }
}

int check_status(enum Status s) {
    if (s == OK) return 1;
    if (s == ERROR) return 0;
    return -1;
}

int main(void) {
    enum Color c = GREEN;
    enum Status s = OK;

    int rgb = color_to_rgb(c);
    int result = check_status(s);

    return rgb + result;
}

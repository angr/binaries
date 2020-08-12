void stop_normal() {
    for (int i = 0; i < 100; i++);
}

void stop_symbolic_read_symbolic_tracking_disabled() {
    int x[100];
    x[0] += 1;
}

void stop_error() {
    // ??
}

void stop_syscall() {
    // tested elsewhere
}

void stop_execnone() {
    // I have no idea how to test this
}

void stop_nostart() {
    // also that
}

char buf[] = {0x41, 0x41, 0x41, 0x41, 0};

void stop_segfault() {
    ((void (*)())buf)();
}

void stop_symbolic_read_addr() {
    long x;
    int y = *(int *)x;
}

void stop_symbolic_write_addr() {
    long x;
    *(int *)x = 1;
}

void stop_symbolic_block_exit() {
    int x[100];
    x[0] += 1;
    if (x[0] == 10) {
        x[1] = 1;
    }
}

void stop_symbolic_condition() {
    // How to generate an VEX ITE expr with symbolic condition?
}

int main(int argc, char** argv) {
    switch (argc) {
        case 1:
            stop_normal();
            break;
        case 2:
            stop_symbolic_read_symbolic_tracking_disabled();
            break;
        case 3:
            stop_error();
            break;
        case 4:
            stop_syscall();
            break;
        case 5:
            stop_execnone();
            break;
        case 6:
            stop_nostart();
            break;
        case 7:
            stop_segfault();
            break;
        case 8:
            stop_symbolic_read_addr();
            break;
        case 9:
            stop_symbolic_write_addr();
            break;
        case 10:
            stop_symbolic_block_exit();
            break;
        case 11:
            stop_symbolic_condition();
            break;
    }
}

void stop_normal() {
    for (int i = 0; i < 100; i++);
}

void stop_symbolic() {
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

int main(int argc, char** argv) {
    switch (argc) {
        case 1:
            stop_normal();
            break;
        case 2:
            stop_symbolic();
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
    }
}

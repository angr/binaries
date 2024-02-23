int main(int argc, char **argv) {
    int a;
    int b;
    int c;
    if(argc == 1) {
        b = 2;
        a = 1;
        c = 3;
    }
    else {
        b = 2;
        c = 3;
    }
    
    puts((char) b);
    puts((char) c);
    return a + b;
}

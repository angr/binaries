char c;
void f1(void) {
        c = 'b';
}

int main(void) {
        c = 'a';
        f1();
        int b = c == 'a';
        return 0;
}


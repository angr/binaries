#include <libcgc.h>

void addstuff(void) {
    int i;
    int array[32];

    receive(0, array, sizeof(array) * 4, NULL);

    for(i=0;i<64;i++) {
        array[i] += 0x1337;
    }

}

int main(void) {
    char arr[1024];

    addstuff();
}

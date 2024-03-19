#include <stdio.h>

int main(int argc, char** argv) {
    int do_reboot = (int) argv[1][0];
    puts(do_reboot);
    if (do_reboot) {
        puts("rebooting");
        while(1);
    }
    puts("goodbye"); 
}

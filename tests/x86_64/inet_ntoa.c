#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    struct in_addr in = {0};
    uint32_t addr = 3232284840; // 192.168.192.168 
    memcpy(&in, &addr, 4);
    char * out = inet_ntoa(in);
    puts(out); // EXPECT: b"192.168.192.168\n"
}
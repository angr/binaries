#include <stdio.h>

int main(){
    char buffer[5];

    read(0, buffer, 4);

    if( buffer[0] == 'a' ) {
       puts("First letter good");
    } else {
       puts("First letter bad");
    }

  return 0;
}

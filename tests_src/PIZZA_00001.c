#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* CGC DECREE binary exploitation test */

static void print_banner()
{
    printf("Welcome to Addr\n");
}

static void print_menu()
{
   printf("1) Add number to the array\n");
   printf("2) Add random number to the array\n");
   printf("3) Sum numbers\n");
   printf("4) Exit\n");
}

void __attribute__((fastcall)) main(int secret_flag_i)
{
  unsigned idx, val;
  char cmd[2];
  int sum = 0;
  int i;
  unsigned iarr[32] = {0};

  void *secret_page = (void *)secret_flag_i;

  while(1) {
     print_menu();
     receive(0, &cmd, 2, NULL);

     switch(cmd[0]) {
	case '1':
		printf("  Index: ");
		receive(0, &idx, 4, NULL);
		if (idx > 0x32) {
			printf("Bad index\n");
			break;
		}
		receive(0, &val, 4, NULL);
		iarr[idx] += val;
		printf("\n");
		break;
	case '2':
		iarr[0] = *((int *)secret_page);
		printf("Randomness added\n");
		break;
	case '3':
		for(i=0;i<sizeof(iarr)/sizeof(int);i++) 
			sum += iarr[i];
		printf("Sum: (");
		transmit(1, &sum, 4, NULL);
		printf(")\n");
		break;
        case '4':
		return;
	default:
		printf("Bad CMD\n");
     }
  }

}

#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

struct stack_func_1 {
  char buf[0x20];
  char *where;
};

size_t receive_n( int fd, void *dst_a, size_t n_bytes )
{
  char *dst = (char *)dst_a;
  size_t len = 0;
  size_t rx = 0;
  while(len < n_bytes) {
    if (receive(fd, dst + len, n_bytes - len, &rx) != 0) {
      len = 0;
      break;
    }
    len += rx;
  }

  return len;
}

int send_all(int fd, const void *msg, size_t n_bytes)
{
  size_t len = 0;
  size_t tx = 0;
  while(len < n_bytes) {
    if (transmit(fd, (char *)msg + len, n_bytes - len, &tx) != 0) {
      return 1;
    }
    len += tx;
  }
  return 0;
}

#define INT_DIGITS 19		/* enough for 64 bit integer */

char *itoa2(i)
     int i;
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2] = {0};
  char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

int myglobal;

int __attribute((fastcall)) main(int secret_page_i) {

  int x;
  char *flag = (void *)secret_page_i;

  for(int i=0;i<20;i++) {
  transmit(1, "A", 1, NULL);
  }
  
  x = *((int*)flag+123);
  char *str_flag = itoa2(x);
  
  transmit(1, str_flag, 12, NULL);

  for(int i=0;i<20;i++) {
  transmit(1, "B", 1, NULL);
  }

  return 0;
}



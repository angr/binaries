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

char
to_hex(unsigned char v)
{
    if (v < 10)
        return '0' + v;
    else if (v < 16)
        return 'a' + (v - 10);
    else
        return '\0';
}

int myglobal;

int __attribute((fastcall)) main(int secret_page_i) {

  int x;
  int i;
  char *flag = (void *)secret_page_i;
  char flag_buf[41];
	
  for(int i=0;i<40;i++) {
	flag_buf[i++] = to_hex((unsigned char) *flag / 16 % 16);
	flag_buf[i] = to_hex((unsigned char) *flag++ % 16);
  }

  for(int i=0;i<20;i++) {
  transmit(1, "A", 1, NULL);
  }
  transmit(1, flag_buf, 40, NULL);

  for(int i=0;i<20;i++) {
  transmit(1, "B", 1, NULL);
  }

  return 0;
}



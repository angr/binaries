#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

int atoi(const char* str)
{
    if ( str == NULL )
        return 0;

    int integer_part = 0;
    int sign = 1;
    int part;
    int digit_count = 0;

    // Skip whitespace
    while ( isspace( str[0] ) )
        str++;

    part = 0; // First part (+/-/number is acceptable)

    while( str[0] != '\0' )
    {
        if ( str[0] == '-' )
        {
            if ( part != 0 )
                return 0;

            sign = -1;
            part++;
        }
        else if ( str[0] == '+' )
        {
            if ( part != 0 )
                return 0;

            part++;
        }
        else if ( isdigit( *str ) )
        {
            if ( part == 0 || part == 1 )
            {
                // In integer part
                part = 1;
                integer_part = (integer_part * 10) + (*str - '0');

                digit_count++;

                if ( digit_count == 9 )
                    break;
            }
            else
            {
                // part invalid
                return 0;
            }
        }
        else
            break;

        str++;
    }

    return (sign * integer_part);
}

size_t receive_until(int fd, char *dst, char delim, size_t max )
{
    size_t len = 0;
    size_t rx = 0;
    char c = 0;

    while( len < max ) {
        dst[len] = 0x00;

        if ( receive( fd, &c, 1, &rx ) != 0 ) {
            len = 0;
            goto end;
        }

        if ( c == delim ) {
            goto end;
        }

        dst[len] = c;
        len++;
    }
end:
    return len;
}

size_t receive_n( int fd, void *dst_buf, size_t n_bytes )
{
  char *dst = dst_buf;
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

unsigned char key = '\0';
char *current_secret = NULL;
int global_secret = 0;

enum action_t { LOAD_KEY, LOAD_SECRET, LOAD_SZ };

void present_key(char *secret) {
	int i;

	for (i=0;i<101;i++) {
		key -= secret[i];
	}

	transmit(0, "+", 1, NULL);
	transmit(0, &key, 1, NULL);
	transmit(0, "+", 1, NULL);
}

void show_secret(char *cs) {
	int sp = global_secret;
	char secretb[0x80];

        strcpy(secretb, cs);

	char *dst = malloc(0x80);

	memcpy(dst, cs, 0x80);

	transmit(0, dst, 0x80, NULL);

	if (global_secret != sp) {
		_terminate(1);
	}
}

void kxor(char *buf, size_t sz) {
 	int i;

	for (i=0;i<sz;i++) {
		buf[i] ^= key;
	}
}

void srecv(char *buf, size_t sz) {
	receive_n(0, buf, sz);

        kxor(buf, sz);
}

void __attribute__((fastcall)) main(int secret_flag_i)
{
	char cmd[1024];
 	size_t n;
	char *secret = (char *)secret_flag_i;

	present_key(secret);

	char buf[10];
	char *lsecret;
        int msize = 0;
	global_secret = ((int *)secret_flag_i)[0];

	while (1) {
		srecv(buf, 1);
		switch(buf[0]) {

		case LOAD_KEY:
			show_secret(current_secret);
			break;

		case LOAD_SECRET:
			if (msize > 0x1000) {
				_terminate(1);
			}
			current_secret = malloc(msize);
			srecv(current_secret, msize);
			break;

		case LOAD_SZ:
			srecv(&msize, 4);
			break;


		}
	}
}

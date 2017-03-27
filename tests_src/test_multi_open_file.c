/* Test for fdopen bugs.  */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#undef assert
#define assert(x) \
  if (!(x)) \
    { \
      fputs ("test failed: " #x "\n", stderr); \
      retval = 1; \
      goto the_end; \
    }

char buffer[256];

int
main (int argc, char *argv[])
{
  char *name;
  FILE *fp = NULL;
  int retval = 0;
  int fd;

  name = tmpnam (NULL);
  fp = fopen (name, "w");
  printf("fopen fd: %d\n", fileno(fp));
  assert (fp != NULL);
  assert (fileno(fp) == 3);
  fputs ("foobar and baz", fp);
  fclose (fp);
  fp = NULL;

  fd = open (name, O_RDONLY);
  printf("open fd: %d\n", fd);
  assert (fd == 3);
  assert (lseek (fd, 5, SEEK_SET) == 5);
  /* The file position indicator associated with the new stream is set to
     the position indicated by the file offset associated with the file
     descriptor.  */
  fp = fdopen (fd, "r");
  printf("fdopen fd: %d\n", fp->_fileno);
  assert (fp != NULL);
  assert (fileno(fp) == 3);
  assert (getc (fp) == 'r');
  assert (getc (fp) == ' ');

the_end:
  if (fp != NULL)
    fclose (fp);
  unlink (name);

  return retval;
}

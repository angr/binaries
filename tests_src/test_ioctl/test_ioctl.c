#include <sys/ioctl.h>
#include <errno.h>
int main()
{
    errno = 0;
    // the only way to make sure the check is exactly the same as in libc is to check the errno
    // This is necessary because we want to make sure no new constraints are introduced.
    // We want exactly 2 branches, syscall success and syscall error
    ioctl(1, 0x1234);
    if (errno != 0) {
        return 1;
    }
    return 0;
}

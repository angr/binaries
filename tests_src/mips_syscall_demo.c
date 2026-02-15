/*
 * Building the Static MIPS Binary
 *
 * Prerequisites
 *
 *   Ubuntu/Debian host (x86_64). An internet connection is required to
 *   install packages.
 *
 * Step 1 -- Install the MIPS cross-compilation toolchain
 *
 *   sudo apt-get update
 *   sudo apt-get install -y gcc-mipsel-linux-gnu libc6-dev-mipsel-cross
 *
 *   This provides mipsel-linux-gnu-gcc (little-endian MIPS) and the static C
 *   library needed for -static linking.
 *
 * Step 2 -- Compile
 *
 *   mipsel-linux-gnu-gcc -static -o syscall_demo syscall_demo.c
 *
 *   -static links glibc (and its syscall wrappers) into the binary so it
 *   has no runtime shared-library dependencies.
 *
 * Step 3 -- Verify
 *
 *   file syscall_demo
 *   # Expected: ELF 32-bit LSB executable, MIPS, MIPS32 rel2 ... statically linked
 *
 *   mipsel-linux-gnu-nm syscall_demo | grep -E '\b(connect|socket|prctl|getsockopt|getsockname|bind)\b'
 *   # All six libc wrappers should appear as symbols.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int fd, ret, optval;
    socklen_t optlen, addrlen;
    struct sockaddr_in addr, peer, local;

    /* 1. socket() — create a TCP socket */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }
    printf("socket() returned fd %d\n", fd);

    /* 2. prctl() — set the process name */
    ret = prctl(PR_SET_NAME, "syscall_demo", 0, 0, 0);
    if (ret < 0) {
        perror("prctl");
    } else {
        printf("prctl(PR_SET_NAME) succeeded\n");
    }

    /* 3. bind() — bind the socket to an ephemeral port on loopback */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(0);  /* let the kernel pick a port */

    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind");
    } else {
        printf("bind() succeeded\n");
    }

    /* 4. getsockname() — retrieve the address the socket is bound to */
    memset(&local, 0, sizeof(local));
    addrlen = sizeof(local);
    ret = getsockname(fd, (struct sockaddr *)&local, &addrlen);
    if (ret < 0) {
        perror("getsockname");
    } else {
        printf("getsockname() returned port %d\n", ntohs(local.sin_port));
    }

    /* 5. getsockopt() — query the SO_REUSEADDR option */
    optval = 0;
    optlen = sizeof(optval);
    ret = getsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen);
    if (ret < 0) {
        perror("getsockopt");
    } else {
        printf("getsockopt(SO_REUSEADDR) = %d\n", optval);
    }

    /* 6. connect() — attempt to connect to localhost:1 (expected to fail) */
    memset(&peer, 0, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    peer.sin_port = htons(1);

    ret = connect(fd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret < 0) {
        printf("connect() failed as expected: %s\n", strerror(errno));
    } else {
        printf("connect() succeeded (unexpected)\n");
    }

    close(fd);
    printf("All syscalls exercised. Done.\n");
    return 0;
}

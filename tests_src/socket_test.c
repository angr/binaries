#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int testcases(int fd){
    char buffer[7];
    char *ask = "more data. ";
    char *output0 = "nope. ";
    char *output1 = "send case 1 works. ";
    char *output2 = "send case 2 works. ";

    recv(fd, buffer, 7, 0);
    if (strcmp(buffer, "case-1\n") == 0) {
        send(fd, output1, strlen(output1), 0);
        return 1;
    } else {
        send(fd, ask, strlen(ask), 0);
        recv(fd, buffer, 7, 0);
        if (strcmp(buffer, "case-2\n") == 0) {
            send(fd, output2, strlen(output2), 0);
            return 2;
        } else {
            send(fd, output0, strlen(output0), 0);
            return 3;
        }
    }

    return;
}

int main() {
    int welcomeSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    setsockopt(welcomeSocket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    listen(welcomeSocket, 1);

    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    testcases(newSocket);

    close(newSocket);
    close(welcomeSocket);

    return 0;
}


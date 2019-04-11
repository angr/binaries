#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



int talk(int fd)
{
   char buf[100];
   recv(fd, buf, 2048, 0);
   return 0;
}

int main(int argc, char** argv)
{

   if (argc != 2)
   {
	  printf("Usage: %s <port>\n", argv[0]);
	  return -1;
   }
   
   int server_fd;
   int opt = 1;
   struct sockaddr_in address;
   int new_client;
   int addrlen;

   server_fd = socket(AF_INET, SOCK_STREAM, 0);

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr("127.0.0.1");
   address.sin_port = htons(atoi(argv[1]));

   if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)))
   {
	  printf("error binding\n");
	  return -1;
   }

   listen(server_fd, 10);
   new_client = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

   talk(new_client);
   close(new_client);
   return 0;
}

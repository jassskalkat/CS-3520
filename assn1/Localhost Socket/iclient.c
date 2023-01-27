#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include "buffer.h"

int main(int argc, char* argv[])
{
	int fd_socket; 
	int len;
	struct sockaddr_in address;
	int result;
	char strcha[MAXsize];

	/* create socket for client*/
	fd_socket=socket(AF_INET, SOCK_STREAM, 0);

	/* Name the socket */
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("127.0.0.1");
	address.sin_port=9110;
	len=sizeof(address);
	
	/* Connect client to server socket*/
	result=connect(fd_socket, (struct sockaddr *)&address,len);
	if (result ==-1)
	{
		perror("client");
		exit(1);
	}

	/*communicate with server*/
    int StillActive=1;
	while (StillActive)
	{
         printf("We need your input: (Type 'stop' for calculations | Type '.' to terminate the program)\n");
         fgets( bufferout, 256, stdin );
         write(fd_socket,bufferout,strlen(bufferout)+1);
         printf("waiting for server...\n");

         if (strcasecmp("stop\n", bufferout) == 0){
             readinbuffer(fd_socket,bufferin);
             printf("%s\n",bufferin);
         }
         StillActive=strcasecmp(bufferout,".\n");
	}
    close(fd_socket);
	exit(0);

}

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "buffer.h"

#define DEFAULT_PROTOCOL 0

void readClient(int fd);
int readline(int fd, char* str,int *m);


int main(int argc, char* argv[]){

    int fd_client, fd_server, serverln,result;
    struct sockaddr_un serverUNIXAddress;
    struct sockaddr* serverSockAddrPtr;

    serverSockAddrPtr=(struct sockaddr*)&serverUNIXAddress;
    serverln=sizeof(serverUNIXAddress);

    fd_client= socket (AF_UNIX,SOCK_STREAM, DEFAULT_PROTOCOL);
    serverUNIXAddress.sun_family= AF_UNIX;
    strcpy(serverUNIXAddress.sun_path,"sockServ");


    /*loop until connection is establised with the server*/
    do{
        result=connect(fd_client,serverSockAddrPtr,serverln);
        if (result<0)         //(result==-1)
        {
            sleep(1);
            printf("client trying to connect  again...\n");
        }

    }
    while (result<0);//(result ==-1);
    printf("Connection Established %d %d\n",fd_client,result);

    int StillActive=1;
    while (StillActive){
        printf("We need your input: (Type 'stop' for calculations | Type '.' to terminate the program)\n");
        fgets( bufferout, 256, stdin );
        printf("Sending %s to server (%d)\n",bufferout, fd_client);
        write(fd_client,bufferout,strlen(bufferout)+1);
        printf("waiting for server...\n");
        if (strcasecmp("stop\n", bufferout) == 0){
            printf("Waiting for server to send back data\n");
            sleep(1);
            readinbuffer(fd_client,bufferin);
            printf("Server sent the following results: %s\n", bufferin);
        }
        StillActive=strcasecmp(bufferout,".\n");
    }
//       readClient(fd_client);
    close(fd_client);
    printf("Client will now terminate\n");
    exit(EXIT_SUCCESS);

}
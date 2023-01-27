#include <string.h>
#include <stdio.h>
#include <unistd.h>


void readinbuffer(int fd_socket,char* buffer)
{
    char * bufferpos;
    bufferpos=buffer-1;
    do
    {
        bufferpos++;
        read(fd_socket,bufferpos,1);
    }
    while (*bufferpos);

    return;
}

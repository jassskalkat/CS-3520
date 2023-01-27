#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "buffer.h"

#define DEFAULT_PROTOCOL 0

int main(int argc, char* argv[]){
    int fd_server, fd_client, serverln,clientln;
    struct sockaddr_un serverUNIXAddress;
    struct sockaddr_un clientUNIXAddress;
    struct sockaddr*   serverSockAddrPtr;
    struct sockaddr*   clientSockAddrPtr;
    int status;
    int errcode;
    int n,index, accessCounter;
    char message[255] = "";
    char q[255] = "";
    char *directions[100] = {NULL};
    FILE * fp;
    fp = fopen("./CoursesHighestMarks.txt", "w+");
    size_t nstr = 0;

    /* ignore death of a child signals to prevent zombies*/
    signal(SIGCHLD,SIG_IGN);
    /* This will set errno to 10, when child process finishes */


    /*setting variables for creating the UNIX socket
      for the server and for the client: address/pointer and its length */
    serverSockAddrPtr=(struct sockaddr*)&serverUNIXAddress;
    serverln = sizeof(serverUNIXAddress);

    clientSockAddrPtr=(struct sockaddr*)&clientUNIXAddress;
    clientln = sizeof(clientUNIXAddress);

    /* create unix socket, bidirectional, default */
    fd_server = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    /* this is the file descriptor corresponding to the server socket
        We remind that everything in UNIX is a file, including sockets
    */
    printf("Server File Number = %d\n",fd_server);

    serverUNIXAddress.sun_family = AF_UNIX;
    /* domain type = AF_UNIX */
    /* set name*/
    strcpy(serverUNIXAddress.sun_path, "sockServ");
    unlink("sockServ"); /* remove if it exists */
    /* creates file (the socket)*/
    errcode=bind(fd_server, serverSockAddrPtr,serverln);
    printf("bind errno= %d, errcode= %d\n",errno,errcode);
    /* the pending connection length(queue length) is 5 */
    errcode=listen (fd_server,5);
    printf("listen errno= %d, errcode= %d\n",errno,errcode);
    clientln=sizeof(clientSockAddrPtr);
    fd_client = accept(fd_server, clientSockAddrPtr,&clientln);
    printf("Accepted Client with File Number =%d\n",fd_client);

    while(1){
        printf("receiving (SERVER)\n");
        readinbuffer(fd_client,bufferin);
        n=strlen(bufferin);
        //printf("I received %s before inserting it in the pointer", bufferin);
        if (strcasecmp(bufferin, "stop\n") != 0){
            directions[nstr] = malloc(n);
            strcpy(directions[nstr], bufferin);
            printf("I received from client: %s\n", directions[nstr]);
            ++nstr;
        }else{

            index = 0;
            int studentIndex = index + 1;
            int numberOfStudents = atoi(&directions[studentIndex][0]);
            int numberOfAssignments = atoi(&directions[numberOfStudents+2][0]);
            char *found;
            int i = 0;
            int j = 0;
            int counter = 0;
            char *percentage;
            int anotherIndex = 0;
            int maxStudentID;
            int max;
            int finalMark;
            accessCounter = 0;
            char *courseName;
            int studentMarks[numberOfStudents][numberOfAssignments+7];

            char str[3];
            char str1[3];

            while (directions[index] != NULL && directions[index+1] != NULL){
                accessCounter++;
                studentIndex = index + 1;
                numberOfStudents = atoi(&directions[studentIndex][0]);
                numberOfAssignments = atoi(&directions[studentIndex+numberOfStudents+1][0]);
                studentMarks[numberOfStudents][numberOfAssignments+7] = 0;
                while(j < numberOfStudents){
                    found = strsep(&directions[index+2+j]," ");
                    if (found == NULL){
                        int sumAssignments = 0;
                        for(int k = 1; k < numberOfAssignments + 1; k++){
                            sumAssignments = sumAssignments + studentMarks[j][k];
                        }

                        for (int x = numberOfAssignments+4; x < numberOfAssignments+ 7; x++){
                            if (anotherIndex == 0){
                                percentage = strsep(&directions[studentIndex + numberOfStudents + 2]," ");
                                studentMarks[j][x] = atoi(percentage);
                            } else{
                                studentMarks[j][x] = studentMarks[0][x];
                            }


                        }

                        finalMark = ((sumAssignments/numberOfAssignments + 0.0)*(studentMarks[j][numberOfAssignments+4]/100.0)) + (studentMarks[j][numberOfAssignments+1]*(studentMarks[j][numberOfAssignments+5]/100.0)) + (studentMarks[j][numberOfAssignments+2]*(studentMarks[j][numberOfAssignments+6]/100.0));

                        studentMarks[j][6] = finalMark;
                        j++;
                        i=0;
                        counter = 0;
                        sumAssignments = 0;
                        anotherIndex++;

                    } else{
                        if (counter == 0){
                            studentMarks[j][i] = j + 1;
                            counter++;
                            i++;
                            continue;
                        }else{
                            studentMarks[j][i] = atoi(found);
                            i++;
                        }
                    }

                }

                max = studentMarks[0][6];
                for (int r = 1; r < numberOfStudents; r++) {
                    if(studentMarks[r][6] > max){
                        max = studentMarks[r][6];
                        maxStudentID = r;
                    }
                }

                courseName = strsep(&directions[index]," ");
                char *chunk[20] = {NULL};
                int l = 0;
                while ((chunk[l] = strsep(&directions[index], " ")) != NULL){
                    l++;
                }

                for (int nn = 0; nn<l; nn++){
                    if (l-nn == 1){
                        fprintf(fp, "%s ", strtok(chunk[nn], "\n"));
                    } else{
                        fprintf(fp, "%s ", chunk[nn]);
                    }

                }
                //fprintf(fp, "%s %d Student%d--", courseName, max, studentMarks[maxStudentID][0]);
                fprintf(fp, "mark %d Student%d ", max, studentMarks[maxStudentID][0]);
                for (int nn = 1; nn <= numberOfAssignments+2; nn++){
                    fprintf(fp, "%d ", studentMarks[maxStudentID][nn]);
                }

                fprintf(fp, "\n");

                sprintf(str, "%d", max);
                sprintf(str1,"%d", studentMarks[maxStudentID][0]);
                strcat(message, courseName);
                strcat(message, " ");
                strcat(message, str);
                strcat(message, " Student");
                strcat(message, str1);
                strcat(message, " ");

                index = index + numberOfStudents + 4;
                j = 0;
                anotherIndex = 0;
            }

            write(fd_client,message,strlen(message)+1);
            continue;

        }

        if (accessCounter != 0){
            if(strcasecmp(directions[index], ".\n") == 0){
                fclose(fp);
            }
            printf("Writing new course to file...\n");
            fprintf(fp, "%s", directions[index]);
            printf("Input written to file successfully\n");
            fclose(fp);
        }

        if(!strcasecmp(bufferin,".\n")){
            break;
        }
    }

    close(fd_client);  /*close socket*/
    return 0;
}
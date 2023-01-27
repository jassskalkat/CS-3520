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
 int fd_server_socket; 
 int fd_client_socket; 
 int server_len;
 int client_len;
 int n,index, accessCounter;
 char message[255] = "";
 char *directions[100] = {NULL};
 FILE * fp;
 fp = fopen("./CoursesHighestMarks.txt", "w+");
 size_t nstr = 0;

 struct sockaddr_in server_address;
 struct sockaddr_in client_address;
 
 /* create an unnamed socket*/
 fd_server_socket=socket(AF_INET, SOCK_STREAM, 0);
 
 /* Name the socket */
 server_address.sin_family=AF_INET;
 server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
 server_address.sin_port=9110;
 server_len=sizeof(server_address);
 bind(fd_server_socket,(struct sockaddr*)& server_address,server_len);

 /* create a connection queue and wait for clients */
 listen(fd_server_socket,15);
 while (1){
	
	printf("server waiting\n");

	/* accept connection*/
        client_len=sizeof(client_address);
        fd_client_socket = accept(fd_server_socket,(struct sockaddr *)&client_address,&client_len);

	/* communicate with client*/
        //printf("Communication started s:%d, c:%d\n",fd_server_socket,fd_client_socket);
	while(1){
        printf("receiving (SERVER)\n");
        readinbuffer(fd_client_socket,bufferin);
        n=strlen(bufferin);

        if (strcasecmp(bufferin, "stop\n") != 0){
            directions[nstr] = malloc(n);
            strcpy(directions[nstr], bufferin);
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


            write(fd_client_socket,message,strlen(message)+1);
            continue;

        }
        if (accessCounter != 0){
            if(strcasecmp(directions[index], ".\n") == 0){
                fclose(fp);
            }
            printf("Writing to file...\n");
            fprintf(fp, "%s", directions[index]);
            printf("Input written to file successfully\n");
            fclose(fp);
        }else {

        }

        if(!strcasecmp(bufferin,".\n")){
            break;
        }

	}
    if(!strcasecmp(bufferin,".\n")){
        break;
    }

 }
	close(fd_client_socket);
 exit(0);

}

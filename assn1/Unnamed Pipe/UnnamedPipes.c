#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {

    int p1[2]; // 0 is read | 1 is write
    int index = 0;
    FILE *fp;
    fp = fopen("./CourseHighestMarks.txt", "w+");
    char message[2048] = "";
    int accessCounter = 0;
    char newCourse[256] = "";
    char *directions[100] = {NULL};
    if (pipe(p1) == -1) { return 1; }
    int pid = fork();
    if (pid == -1) { return 2; }

    if (pid == 0) {
        //Child process
        char *directions[100] = {NULL};
        int l = 0;
        char x[4096] = "";
        while (1) {
            if (read(p1[0], x, sizeof(x) + 1) == -1) { return 3; }
            printf("Received from client: %s\n", x);
            if (strcasecmp(x, ".\n") == 0) {
                printf("Input ended.\n");
                break;
            } else {
                directions[l] = malloc(strlen(x));
                strcpy(directions[l], x);
                l++;
            }
        }
        printf("Received data successfully\n");

        int studentIndex = index + 1;
        int numberOfStudents = atoi(&directions[studentIndex][0]);
        int numberOfAssignments = atoi(&directions[numberOfStudents + 2][0]);
        char *found;
        int i = 0;
        //int studentMarks[numberOfStudents][numberOfAssignments+7];  // ID - ASN - ASN - ASN - MIDTRM - FINAL - OVERALL MARK - Percentage 1 - Percentage 2 - Percentage 3
        int j = 0;
        int counter = 0;
        char *percentage;
        int anotherIndex = 0;
        int maxStudentID;
        int max;
        int finalMark;
        char *courseName;
        int studentMarks[numberOfStudents][numberOfAssignments + 7];

        char str[3];
        char str1[3];

        while (directions[index] != NULL && directions[index + 1] != NULL) {
            accessCounter++;
            studentIndex = index + 1;
            numberOfStudents = atoi(&directions[studentIndex][0]);
            numberOfAssignments = atoi(&directions[studentIndex + numberOfStudents + 1][0]);
            studentMarks[numberOfStudents][numberOfAssignments + 7] = 0;
            while (j < numberOfStudents) {
                found = strsep(&directions[index + 2 + j], " ");
                if (found == NULL) {
                    int sumAssignments = 0;
                    for (int k = 1; k < numberOfAssignments + 1; k++) {
                        sumAssignments = sumAssignments + studentMarks[j][k];
                    }

                    for (int x = numberOfAssignments + 4; x < numberOfAssignments + 7; x++) {
                        if (anotherIndex == 0) {
                            percentage = strsep(&directions[studentIndex + numberOfStudents + 2], " ");
                            studentMarks[j][x] = atoi(percentage);
                        } else {
                            studentMarks[j][x] = studentMarks[0][x];
                        }


                    }

                    finalMark = ((sumAssignments / numberOfAssignments + 0.0) *
                                 (studentMarks[j][numberOfAssignments + 4] / 100.0)) +
                                (studentMarks[j][numberOfAssignments + 1] *
                                 (studentMarks[j][numberOfAssignments + 5] / 100.0)) +
                                (studentMarks[j][numberOfAssignments + 2] *
                                 (studentMarks[j][numberOfAssignments + 6] / 100.0));

                    studentMarks[j][6] = finalMark;
                    j++;
                    i = 0;
                    counter = 0;
                    sumAssignments = 0;
                    anotherIndex++;

                } else {
                    if (counter == 0) {
                        studentMarks[j][i] = j + 1;
                        counter++;
                        i++;
                        continue;
                    } else {
                        studentMarks[j][i] = atoi(found);
                        i++;
                    }
                }

            }

            max = studentMarks[0][6];
            for (int r = 1; r < numberOfStudents; r++) {
                if (studentMarks[r][6] > max) {
                    max = studentMarks[r][6];
                    maxStudentID = r;
                }
            }

            courseName = strsep(&directions[index], " ");
            char *chunk[20] = {NULL};
            int l = 0;
            while ((chunk[l] = strsep(&directions[index], " ")) != NULL) {
                l++;
            }

            for (int nn = 0; nn < l; nn++) {
                if (l - nn == 1) {
                    fprintf(fp, "%s ", strtok(chunk[nn], "\n"));
                } else {
                    fprintf(fp, "%s ", chunk[nn]);
                }

            }
            fprintf(fp, "mark %d Student%d ", max, studentMarks[maxStudentID][0]);
            for (int nn = 1; nn <= numberOfAssignments + 2; nn++) {
                fprintf(fp, "%d ", studentMarks[maxStudentID][nn]);
            }

            fprintf(fp, "\n");

            sprintf(str, "%d", max);
            sprintf(str1, "%d", studentMarks[maxStudentID][0]);
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

        printf("Sending answer back to client...\n");
        write(p1[1], message, strlen(message) + 1);
        printf("Data sent successfully\n");
    }
     else {
        //Parent process
        const MAX_LINE_SIZE = 4096;
        char line[MAX_LINE_SIZE];
        size_t nstr = 0;
        int stillActive = 1;
        while (stillActive) {
            fgets(line, MAX_LINE_SIZE, stdin);
            if (write(p1[1], line, sizeof(line) + 1) == -1) { return 5; }
            printf("Writing to client...\n");

            if (strcasecmp(".\n", line) == 0){
                break;
            }
        }
	sleep(1);
	read(p1[0], line, sizeof(line));
        printf("I received from the server: %s\n", line);
        printf("Do you have any new courses to add?\n");
	fgets(line, MAX_LINE_SIZE, stdin);
        printf("New course I received from user is %s\nWriting it to file...\n", line);
        fprintf(fp, "%s", line);
        printf("Wrote to file successfully.\nExiting program now.\n");
    }
    fclose(fp);
    close(p1[0]);
    close(p1[1]);
    return 0;
}

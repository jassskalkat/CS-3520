#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int numberOfStudents;
int **students;
int **assignments;
int **midterms;


void *input();
void *calculateAssignments();
void *calculateMidterms();
void *calculateFinal();
void *calculateTotalStudentGrade();
void *findMaxAndDisplayIt();

void *input()
{

    printf("Please enter the number of students: ");
    scanf("%d", &numberOfStudents);

    students = malloc( numberOfStudents*sizeof(int *));
    for (int i = 0 ; i < numberOfStudents;i++){
        printf("Please enter your information in this format line by line:\n'Number of assignments - number of midterms - weight of assignments - weight of midterms - weight of final'\n");
        printf("Passing data for Student %d\n", i+1);
        students[i] = malloc( 9*sizeof(int));
        for (int j = 0; j < 5; j++){
            scanf("%d", &students[i][j]);
        }
    }


    assignments = malloc( numberOfStudents*sizeof(int *));
    for (int i = 0 ; i < numberOfStudents;i++){
        printf("Please enter your assignments grade for student %d:\n", i+1);
        assignments[i] = malloc( students[i][0]*sizeof(int));
        for (int j = 0; j < students[i][0]; j++){
            scanf("%d", &assignments[i][j]);
        }
    }

    midterms = malloc( numberOfStudents*sizeof(int *));
    for (int i = 0 ; i < numberOfStudents;i++){
        printf("Please enter your midterms grade for student %d:\n", i+1);
        midterms[i] = malloc( students[i][1]*sizeof(int) );
        for (int j = 0; j < students[i][1]; j++){
            scanf("%d", &midterms[i][j]);
        }
    }
    for (int i = 0; i < numberOfStudents; i++){
        printf("Please enter the final grade of Student %d: ", i+1);
        scanf("%d", &students[i][7]);
    }

    pthread_exit(NULL);
}

void *calculateAssignments(){

    double sumOfAssignments = 0;

    for(int i = 0; i < numberOfStudents; i++){
        for (int j = 0; j< students[i][0]; j++){
            sumOfAssignments += assignments[i][j];
        }
        sumOfAssignments = ((sumOfAssignments / students[i][0]) * (students[i][2]/100.0));
        students[i][5] = ceil(sumOfAssignments);
        sumOfAssignments = 0;
    }
    pthread_exit(NULL);
}

void *calculateMidterms(){

    double sumOfMidterms = 0;

    for(int i = 0; i < numberOfStudents; i++){
        for (int j = 0; j< students[i][1]; j++){
            sumOfMidterms += midterms[i][j];
        }
        sumOfMidterms = ((sumOfMidterms / students[i][1]) * (students[i][3]/100.0));
        students[i][6] = ceil(sumOfMidterms);
        sumOfMidterms = 0;
    }
    pthread_exit(NULL);
}

void *calculateFinal(){

    for(int i = 0; i < numberOfStudents; i++){
        students[i][7] = ceil(students[i][7] * (students[i][4]/100.0));
    }

    pthread_exit(NULL);
}

void *calculateTotalStudentGrade(){

    for (int i = 0; i < numberOfStudents; i++){
        students[i][8] = students[i][5] + students[i][6] + students[i][7];
    }

    pthread_exit(NULL);
}

void *findMaxAndDisplayIt(){

    int max = students[0][8];
    int maxStudentID;
    for (int i = 1; i < numberOfStudents; i++) {
        if(students[i][8] > max){
            max = students[i][8];
            maxStudentID = i;
        }
    }
    printf("The student with the highest grade is Student %d with an overall of %d", maxStudentID+1, students[maxStudentID][8]);
    pthread_exit(NULL);
}


int main(){

    pthread_t t1, t2, t3, t4, t5, t6;

    pthread_create(&t1, NULL, input, NULL);
    pthread_join(t1, NULL);


    pthread_create(&t2, NULL, calculateAssignments, NULL);
    pthread_create(&t3, NULL, calculateMidterms, NULL);
    pthread_create(&t4, NULL, calculateFinal, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);


    pthread_create(&t5, NULL, calculateTotalStudentGrade, NULL);
    pthread_join(t5, NULL);

    pthread_create(&t6, NULL, findMaxAndDisplayIt, NULL);
    pthread_join(t6, NULL);

    return 0;
}






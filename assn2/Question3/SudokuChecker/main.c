#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


int Flag = 1;
typedef struct {
    int row;
    int column;
} parameters;

int grid[9][9];

void readGridFromFile(char *ip_file){
    FILE *fp;
    int i, j;
    fp = fopen(ip_file, "r");
    for (i=0; i<9; i++)
        for (j=0; j<9; j++) {
            fscanf(fp, "%d", &grid[i][j]);
        }
}

void* checkColumn(void* columnArgs) {
    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= ((parameters*) columnArgs)->row; i++) {
        int currValue = grid[i][((parameters*) columnArgs)->column];
        if (allValues[currValue] == 0) {
            allValues[currValue] = 1;
        } else {
            Flag = 0;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

void* checkRow(void* rowArgs) {
    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= ((parameters*) rowArgs)->column; i++) {
        int currValue = grid[((parameters*) rowArgs)->row][i];
        if (allValues[currValue] == 0) {
            allValues[currValue] = 1;
        } else {
            Flag = 0;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}


void* checkSubGrid(void* subGridArgs) {
    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = ((parameters*) subGridArgs)->row - 2; i <= ((parameters*) subGridArgs)->row; i++) {
        for (int j = ((parameters*) subGridArgs)->column - 2; j <= ((parameters*) subGridArgs)->column; j++) {
            int currValue = grid[i][j];
            if (allValues[currValue] == 0) {
                allValues[currValue] = 1;
            } else {
                Flag = 0;
                pthread_exit(NULL);
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    readGridFromFile(argv[1]);

    int size = 9; // size
    int s = 3; //sqrt

    for (int i = 0; i <= size-1; i++) {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        parameters* data = (parameters*)malloc(sizeof(parameters));
        data->row = 8;
        data->column = i;
        pthread_create(&tid, &attr, checkColumn, (void*) data);
        pthread_join(tid, NULL);
    }
    for (int i = 0; i <= size-1; i++) {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        parameters* data = (parameters*)malloc(sizeof(parameters));
        data->row = i;
        data->column = 8;
        pthread_create(&tid, &attr, checkRow, (void*) data);
        pthread_join(tid, NULL);
    }
    for (int i = 0; i <= size-1; i += s) {
        for (int j = 0; j <= size-1; j += s) {
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            parameters* data = (parameters*)malloc(sizeof(parameters));
            data->row = j + 2;
            data->column = i + 2;
            pthread_create(&tid, &attr, checkSubGrid, (void*) data);
            pthread_join(tid, NULL);
        }
    }
    printf("Sudoku Puzzle is: ");
    if (Flag == 0) {
        printf("Incorrect");
    } else {
        printf("Correct");
    }
    printf("\n");
}



# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <pthread.h>
# include <stdbool.h>


int grid[25][25], size;


void readGridFromFile(char *ip_file){
	FILE *fp;
	int i, j;
	fp = fopen(ip_file, "r");
	for (i=0; i<size; i++)
		for (j=0; j<size; j++) {
			fscanf(fp, "%d", &grid[i][j]);
	    }
}

void printGrid(){
	int i, j;
	for (i=0; i<size; i++) {
		for (j=0; j<size; j++)
			printf("%d\t", grid[i][j]);
		printf("\n");
	}
}

int sqr(int size){
	if(size==1) return 1;
	else if(size==4) return 2;
	else if(size==9) return 3;
	else if(size==16) return 4;
	else if(size==25) return 5;
	else if(size==36) return 6;
	else return 0;
}

bool findEmptyPlace(int *row, int *col){
    for (*row = 0; *row < size; (*row)++)
        for (*col = 0; *col < size; (*col)++)
            if (grid[*row][*col] == 0){
                return true;
            }
    return false;
}

typedef struct{
	int row;
	int col;
	int size;
	int num;
	int (*grid)[25];
}  cells;

void * inrows(void * cell) {
    cells * data = (cells *) cell;
	int col = data->col;
	int row = data->row;
	int num = data->num;
	int size = data->size;
    for (int col = 0; col < size; col++){
		int val = data->grid[row][col];
		if (val == num) return (void *) 0;
	}
    return (void *) 1;
}

void * incols(void * cell) {
    cells * data = (cells *) cell;
	int col = data->col;
	int row = data->row;
	int num = data->num;
	int size = data->size;
    for (int row = 0; row < size; row++){
		int val = data->grid[row][col];
		if (val == num) return (void *) 0;
	}
    return (void *) 1;
}

void * insub(void * cell) {
    cells * data = (cells *) cell;
	int col = data->col;
	int row = data->row;
	int num = data->num;
	int size = data->size;
	int s = sqr(size);
	int startRow = row - row % s;
	int startCol = col - col % s;
	for (int r = 0; r < s; r++)
		for (int c = 0; c < s; c++){
            int val = data->grid[r + startRow][c + startCol];
            if (val == num) return (void *) 0;
        }
	return (void *) 1;
}


bool validate(int row, int col, int num){

    cells * cell = (cells *) malloc(sizeof(cells));
	cell->size=size;
	cell->row=row;
	cell->col=col;
	cell->grid=grid;
	cell->num=num;


	pthread_t Row, Col, Sub;
	pthread_create(&Row, NULL, inrows, (void *) cell);
    pthread_create(&Col, NULL, incols, (void *) cell);
    pthread_create(&Sub, NULL, insub, (void *) cell);
    void * inRow;
    void * inCol;
    void * inSub;
	pthread_join(Row, &inRow);
    pthread_join(Col, &inCol);
    pthread_join(Sub, &inSub);

	if((intptr_t) inRow==1 && (intptr_t) inCol==1 && (intptr_t) inSub==1){
        return true;
    }
    return false;
}


bool sudokuSolver();

bool sudokuSolver(){
    int row, col;
    if (!findEmptyPlace(&row, &col)) return true;
    for (int num = 1; num <= size; num++){
        if(validate(row, col, num)){
            grid[row][col] = num;
            if(sudokuSolver()){
                return true;
            }
            grid[row][col] = 0;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {

	if (argc != 3) {
		exit(-1);
	}

	size = atoi(argv[1]);
	readGridFromFile(argv[2]);

	if(sudokuSolver()) {
        printGrid();
    }else {
        printf("No solution exists\n");
    }
    return 0;
}
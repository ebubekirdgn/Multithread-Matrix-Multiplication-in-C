#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct MATRIX
{
    int **data;
    int rows;
    int columns;
    double elapsedTime;
} MATRIX;
typedef struct THREAD_DATAS
{
    MATRIX A,B;
    MATRIX *result;// = (MATRIX *) malloc(sizeof(MATRIX));
    int row;
    int column;
} THREAD_DATAS;


MATRIX* read_matrices();

int** fillMatrix(FILE *f, int rows, int columns);

//INPUT OUTPUT METHOD
void writeOutput(MATRIX *matrices);
void printMatrix(MATRIX matrix, char name);

void serial_matrix_multiplication(MATRIX A, MATRIX B, MATRIX *C);
// Element by element threading
void element_by_element(MATRIX A, MATRIX B, MATRIX *C);
void* evaluate_element_thread(void *ptr);

// Element evaluation
void calculate_element(MATRIX A, MATRIX B, MATRIX* C, int row, int column);
 
MATRIX* read_matrices()
{
 	MATRIX  *matrices = (MATRIX *) malloc(2 * sizeof(MATRIX));
    
	FILE *file = fopen("a300.txt", "r");
    
	// FILL MATRICE
    
	fscanf(file, "%d %d\n", &matrices[0].rows, &matrices[0].columns);
    matrices[0].data = fillMatrix(file, matrices[0].rows, matrices[0].columns);
    fscanf(file, "%d %d\n", &matrices[1].rows, &matrices[1].columns);
    matrices[1].data = fillMatrix(file, matrices[1].rows, matrices[1].columns);
    
	fclose(file);
	return matrices;
}

int** fillMatrix(FILE *file, int rows, int columns)
{
    int **matrix = (int**) malloc(rows * sizeof(int *));
    int *newRow;
    int i,j;
    for (i = 0; i < rows; i++)
    {
        newRow = (int*) malloc(columns * sizeof(int));
        for(j = 0; j < (columns-1) ; j++)
        {
            fscanf(file, "%d ", &newRow[j]);
        }
        fscanf(file, "%d\n", &newRow[j]);
        matrix[i] = newRow;
    }
    return matrix;
}

void writeOutput(MATRIX *matrices)
{
    FILE *f = fopen("resultparallel.txt", "w");
    int count, row, column;
    for (count = 0; count < 3; count++)
    {
        fprintf(f, "%d\n", (matrices[count].rows * matrices[count].columns));
        for (row = 0; row < matrices[count].rows; row++)
        {
            for (column = 0; column < matrices[count].columns; column++)
                fprintf(f, "%d ", matrices[count].data[row][column]);
            fprintf(f, "\n");
        }
        fprintf(f, "Elapsed time: %.0lf micro sec.\n", matrices[count].elapsedTime);
    }
    fclose(f);
}


void printMatrix(MATRIX matrix, char name)
{
    int rows = matrix.rows;
    int columns = matrix.columns;
    printf("matrix %c (size: %d x %d)\n", name, rows, columns);
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
            printf("%d\t", matrix.data[i][j]);
        printf("\n");
    }
    return;
}

void serial_matrix_multiplication(MATRIX A, MATRIX B, MATRIX *C)
{
    (*C).rows = A.rows;
    (*C).columns = B.columns;
    (*C).data = (int**) malloc(A.rows * sizeof(int*));
    int x, y;
    for (x = 0; x < A.rows; x++)
    {
        (*C).data[x] = (int*) malloc(B.columns * sizeof(int));
        for (y = 0; y < B.columns; y++)
        {
            calculate_element(A, B, C, x, y);
        }
    }
    return;
}


void element_by_element(MATRIX A, MATRIX B, MATRIX *C)
{
    (*C).rows = A.rows;
    (*C).columns = B.columns;
    (*C).data = (int**) malloc(A.rows * sizeof(int*));
    int x, y;
    int z=0;
    printf("\n Enter the thread value (1-2-4-8-16): ");
    scanf("%d",&z);
	
    int threads_number = A.rows * B.columns;
    pthread_t threads[threads_number];
    for (x = 0; x < A.rows; x++)
    {
        (*C).data[x] = (int*) malloc(B.columns * sizeof(int));
        for (y = 0; y < B.columns; y++)
        {
        
            // create & initialize thread struct
            THREAD_DATAS *args = (THREAD_DATAS*) malloc(sizeof(THREAD_DATAS));
            (*args).A = A; (*args).B = B; (*args).result = C; (*args).row = x;  (*args).column = y;
            // create thread
            pthread_create(&threads[z], NULL, evaluate_element_thread, (void*) args);
            z++;
        }
        
    }
    for(z = 0; z < threads_number; z++)
    {
        pthread_join(threads[z], NULL);
    }
    return;
}

void* evaluate_element_thread(void *ptr)
{
    THREAD_DATAS data = *((THREAD_DATAS *) ptr);
    
    (*data.result).data[data.row][data.column] = 0;
    int i;
    for (i = 0; i < data.A.columns; i++)
    {
        (*(data.result)).data[data.row][data.column] += data.A.data[data.row][i] * data.B.data[i][data.column];
    }
    return NULL;
}

// Element evaluation
void calculate_element(MATRIX A, MATRIX B, MATRIX* C, int row, int column)
{
    (*C).data[row][column] = 0;
    int i;
    for (i = 0; i < A.columns; i++)
    {
        (*C).data[row][column] += A.data[row][i] * B.data[i][column];
    }
    return;
}

int main()
{
    clock_t startTime, endTime = 0;
    double elapsedTime = 0;
	startTime = clock();
    MATRIX *matrices = read_matrices();
    if(matrices[0].columns != matrices[1].rows)
    {
        printf("OOPPSS!!! ROWS AND COLUMNS DO NOT MATCH WITH EACH OTHER\n");
        exit(0);
    }
    
	printf("\nReading completed successfully\n");

    endTime = clock();
    
    elapsedTime = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC; 
	printf("Load Files Time: %.0lf micro sec.\n", elapsedTime);

    
    printf("\n**************************************************************\n");
    // get results
    MATRIX* result = (MATRIX *)malloc(3 * sizeof(MATRIX));
    startTime, endTime = 0;
    elapsedTime = 0;
    
	// normal matrix multiplication

	printf("COMPARISON WITH THREAD AND WITHOUT THREAD \n\n");
    printf("\n ------> WITHOUT THREAD MATRIX MULTIPLACTION <--------- \n");
    
	startTime = clock();
    serial_matrix_multiplication(matrices[0], matrices[1], &result[0]);
    endTime = clock();
    elapsedTime = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    result[0].elapsedTime = elapsedTime;
    
	//printMatrix(result[0], 'X');
    printf("TOTAL TIME TO COMPUTE THE RESULT (SERIAL): %.0lf micro sec.\n", elapsedTime);
    // threading each for each element in the resulting matrix
    
    printf("\n ------> USING THREAD MATRIX MULTIPLICATION <--------- \n\n");
    startTime = clock();
    element_by_element(matrices[0], matrices[1], &result[1]);
    endTime = clock();
    elapsedTime = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    result[1].elapsedTime = elapsedTime;
    printf("..................  THAT'S ALL ............\n");
 
    printf("TOTAL TIME TO COMPUTE THE RESULT (PARALLEL) : %.0lf micro sec.\n", elapsedTime);

    writeOutput(result);
	return 0;
}

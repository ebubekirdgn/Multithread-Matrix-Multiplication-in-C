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
    MATRIX M,N;
    MATRIX *result;
    int row;
    int column;
    
} THREAD_DATAS;


MATRIX* read_matrices();

int** fill_matrix(FILE *f, int rows, int columns);

//INPUT OUTPUT METHOD
void write_output(MATRIX *matrices);
void print_matrix(MATRIX matrix, char name);

void serial_matrix_multiplication(MATRIX M, MATRIX N, MATRIX *C);
// Element by element threading
void element_by_element(MATRIX M, MATRIX N, MATRIX *C);
void* evaluate_element_thread(void *ptr);

// Element evaluation
void calculate_element(MATRIX M, MATRIX N, MATRIX* C, int row, int column);
 
MATRIX* read_matrices()
{
 	MATRIX  *matrices = (MATRIX *) malloc(2 * sizeof(MATRIX));
    
	FILE *file = fopen("b256.txt", "r");
   
   
    if(file != NULL)
    {    	
		fscanf(file, "%d %d\n", &matrices[0].rows, &matrices[0].columns);
		// FILL MATRICE
	    matrices[0].data = fill_matrix(file, matrices[0].rows, matrices[0].columns);
	    fscanf(file, "%d %d\n", &matrices[1].rows, &matrices[1].columns);
	    matrices[1].data = fill_matrix(file, matrices[1].rows, matrices[1].columns);
	    
		fclose(file);
		return matrices;
	    printf("Dosya Var reis");
	}
	else
	{
   	    printf("FILE NOT FOUND ! PLEASE TRY AGAIN ");
		exit(0);	
	}
    
}

int** fill_matrix(FILE *file, int rows, int columns)
{
    int **matrix = (int**) malloc(rows * sizeof(int *));
    int *new_row;
    int i,j;
    for (i = 0; i < rows; i++)
    {
        new_row = (int*) malloc(columns * sizeof(int));
        for(j = 0; j < (columns-1) ; j++)
        {
            fscanf(file, "%d ", &new_row[j]);
        }
        fscanf(file, "%d\n", &new_row[j]);
        matrix[i] = new_row;
    }
    return matrix;
}

void write_output(MATRIX *matrices)
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


void print_matrix(MATRIX matrix, char name)
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

void serial_matrix_multiplication(MATRIX M, MATRIX N, MATRIX *C)
{
    (*C).rows = M.rows;
    (*C).columns = N.columns;
    (*C).data = (int**) malloc(M.rows * sizeof(int*));
    int x, y;
    for (x = 0; x < M.rows; x++)
    {
        (*C).data[x] = (int*) malloc(N.columns * sizeof(int));
        for (y = 0; y < N.columns; y++)
        {
            calculate_element(M, N, C, x, y);
        }
    }
    return;
}

void element_by_element(MATRIX M, MATRIX N, MATRIX *C)
{
    (*C).rows = M.rows;
    (*C).columns = N.columns;
    (*C).data = (int**) malloc(M.rows * sizeof(int*));
    int x, y;
    int z=0;
    printf("\n Enter the thread value (1-2-4-8-16): ");
    scanf("%d",&z);
	
    int threads_number = M.rows * N.columns;
    pthread_t threads[threads_number];
    for (x = 0; x < M.rows; x++)
    {
        (*C).data[x] = (int*) malloc(N.columns * sizeof(int));
        for (y = 0; y < N.columns; y++)
        {
        
            // create & initialize thread struct
            THREAD_DATAS *args = (THREAD_DATAS*) malloc(sizeof(THREAD_DATAS));
            (*args).M = M; (*args).N = N; (*args).result = C; (*args).row = x;  (*args).column = y;
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
    for (i = 0; i < data.M.columns; i++)
    {
        (*(data.result)).data[data.row][data.column] += data.M.data[data.row][i] * data.N.data[i][data.column];
    }
    return NULL;
}

// Element evaluation
void calculate_element(MATRIX M, MATRIX N, MATRIX* C, int row, int column)
{
    (*C).data[row][column] = 0;
    int i;
    for (i = 0; i < M.columns; i++)
    {
        (*C).data[row][column] += M.data[row][i] * N.data[i][column];
    }
    return;
}

int main()
{
    clock_t startTime, endTime,total_completion ,end_total_completion_time= 0;
    double elapsed_time = 0;
    end_total_completion_time= clock();
	
	startTime = clock();
    
	MATRIX *matrices = read_matrices();
    
    if(matrices[0].columns != matrices[1].rows)
    {
        printf("OOPPSS!!! ROWS AND COLUMNS DO NOT MATCH WITH EACH OTHER\n");
        exit(0);
    }
    
	printf("\nReading completed successfully\n");

    endTime = clock();
    
    elapsed_time = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC; 
	printf("\n FILE LOADING TIME : %.0lf micro sec.\n", elapsed_time);

    printf("\n**************************************************************\n");
    
	// RESULTS MATRICES
    MATRIX* result = (MATRIX *)malloc(3 * sizeof(MATRIX));
    startTime, endTime = 0;
    elapsed_time = 0;
    
	// SERIAL MATRIX

	printf("COMPARISON WITH THREAD AND WITHOUT THREAD \n\n");
    printf("\n ------> WITHOUT THREAD MATRIX MULTIPLACTION <--------- \n");
    
	startTime = clock();
    serial_matrix_multiplication(matrices[0], matrices[1], &result[0]);
    endTime = clock();
    elapsed_time = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    result[0].elapsedTime = elapsed_time;
    
    
    printf("TOTAL TIME TO COMPUTE THE RESULT (SERIAL): %.0lf micro sec.\n", elapsed_time);
    // threading each for each element in the resulting matrix
    
    printf("\n ------> USING THREAD MATRIX MULTIPLICATION <--------- \n\n");
    startTime = clock();
    element_by_element(matrices[0], matrices[1], &result[1]);
    endTime = clock();
    elapsed_time = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    result[1].elapsedTime = elapsed_time;
 
    printf("\nTOTAL TIME TO COMPUTE THE RESULT (PARALLEL) : %.0lf micro sec.\n", elapsed_time);
    
	end_total_completion_time = clock();
	elapsed_time = 1000000.0 * ((double) (endTime - startTime)) / CLOCKS_PER_SEC; 
	printf("\n END TOTAL COMPLETION TIME : %.0lf micro sec.\n", elapsed_time);
	
    write_output(result);
    
	return 0;
}

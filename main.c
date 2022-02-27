#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAT_SIZE 10
#define MAX_THREADS 100

int i,j,k;           //Parameters For Rows And Columns
int matrix1[MAT_SIZE][MAT_SIZE]; //First Matrix
int matrix2[MAT_SIZE][MAT_SIZE]; //Second Matrix
int result [MAT_SIZE][MAT_SIZE]; //Multiplied Matrix

//Type Defining For Passing Function Argumnents
typedef struct parameters {
    int x,y;
}args;

//Function For Calculate Each Element in Result Matrix Used By Threads - - -//
void* mult(void* arg){
    
    args* p = arg;
    int a=0;
    //Calculating Each Element in Result Matrix Using Passed Arguments
    for(a=0;a<j;a++){
        result[p->x][p->y] += matrix1[p->x][a]*matrix2[a][p->y];
    	}
    sleep(3);
    
    //End Of Thread
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    int x,y=0;
    //Initializing All Defined Matrices By Zero - - - - - - - - - - - - - - -//
    for(x=0;x<10;x++){
        for(y=0;y<10;y++){
            matrix1[x][y] = 0;
            matrix2[x][y] = 0;
            result[x][y] = 0;
        }
    }
    //Getting Matrix1 And Matrix2 Info from User - - - - - - - - - - - - - - -//
   
    //printf(" --- Defining Matrix 1 ---\n\n");
    
    // Getting Row And Column(Same As Row In Matrix2) Number For Matrix1


    printf("Enter number of rows for matrix 1: ");
    scanf("%d",&i);
    printf("Enter number of columns for matrix 1: ");
    scanf("%d",&j);
    
    printf("\n --- Initializing Matrix 1 ---\n\n");
    int b,c=0;
    
    char buffer[10];
    char filename[] = "a";

    sprintf(buffer, "%s%d.txt",filename,i);    
    
    FILE *dosya1 = fopen(buffer, "w");
	for(b=0;b<i;b++){
        for(c=0;c<j;c++){
           // printf("Enter variable [%d,%d]: ",b+1,c+1);
           // scanf("%d",&matrix1[b][c]);
             matrix1[b][c] = rand() % 10 + 1;
             fprintf(dosya1, "%d ",matrix1[b][c]);
        }
    }
    fclose(dosya1);
    
    //printf("\n --- Defining Matrix 2 ---\n\n");

    // Getting Column Number For Matrix2
    printf("Number of rows for matrix 2 : %d\n",j);
    printf("Enter number of columns for matrix 2: ");
    scanf("%d",&k);
    
    printf("\n --- Initializing Matrix 2 ---\n\n");
    int d,e=0;
    
	char buffer2[10];
    char filename2[] = "b";

    sprintf(buffer2, "%s%d.txt",filename2,j);    
    
    FILE *dosya2 = fopen(buffer2, "w");
	
	
	for(d=0;d<j;d++){
        for(e=0;e<k;e++){
            //printf("Enter variable [%d,%d]: ",d+1,e+1);
            //scanf("%d",&matrix2[d][e]);
            matrix2[d][e] = rand() % 10 + 1;
            fprintf(dosya2, "%d ",matrix2[d][e]);
        }
    }
    fclose(dosya2);
    
    //Printing Matrices - - - - - - - - - - - - - - - - - - - - - - - - - - -//
    
    printf("\n --- Matrix 1 ---\n\n");
    int f,g=0;
	for(f=0;f<i;f++){
        for(g=0;g<j;g++){
            printf("%5d",matrix1[f][g]);
        }
        printf("\n\n");
    }
    
    printf(" --- Matrix 2 ---\n\n");
    
	int h,i=0;
	for(h=0;h<j;h++){
        for(i=0;i<k;i++){
            printf("%5d",matrix2[h][i]);
        }
        printf("\n\n");
    }
    
    //Multiply Matrices Using Threads - - - - - - - - - - - - - - - - - - - -//
    
    //Defining Threads
    pthread_t thread[MAX_THREADS];
    
    //Counter For Thread Index
    int thread_number = 0;
    
    //Defining p For Passing Parameters To Function As Struct
    args p[i*k];
    
    //Start Timer
    time_t start = time(NULL);
    
    for(x=0;x<i;x++){
        for(y=0;y<k;y++){
            
            //Initializing Parameters For Passing To Function
            p[thread_number].x=x;
            p[thread_number].y=y;
            
            //Status For Checking Errors
            int status;
            
            //Create Specific Thread For Each Element In Result Matrix
            status = pthread_create(&thread[thread_number], NULL, mult, (void *) &p[thread_number]);
            
            //Check For Error
            if(status!=0){
                printf("Error In Threads");
                exit(0);
            }   
            thread_number++;
        }
    }
    
    //Wait For All Threads Done - - - - - - - - - - - - - - - - - - - - - - //
    int l =0;
    for(l=0;l<(i*k);l++)
        pthread_join(thread[l],NULL );
    
    //Print Multiplied Matrix (Result) - - - - - - - - - - - - - - - - - - -//
	
	char buffer3[10];
    char filename3[] = "result";

    sprintf(buffer3, "%s%d.txt",filename3,i);    
    
    FILE *multiplied_matrix = fopen(buffer3, "w");

    printf(" --- Multiplied Matrix ---\n\n");
    for(x=0;x<i;x++){
        for(y=0;y<k;y++){
            printf("%5d",result[x][y]);
			fprintf(multiplied_matrix, "%d ",result[x][y]);
        }
        printf("\n\n");
    }
    fclose(multiplied_matrix);
    
    
    //Calculate Total Time Including 3 Soconds Sleep In Each Thread - - - -//
    
    printf(" ---> Time Elapsed : %.2f Sec\n\n", (double)(time(NULL) - start));
    
    //Total Threads Used In Process - - - - - - - - - - - - - - - - - - - -//
    
    printf(" ---> Used Threads : %d \n\n",thread_number);
    int j=0;
	for(j=0;j<thread_number;j++)
        printf(" - Thread %d ID : %d\n",j+1,(int)thread[j]);
    
    return 0;
}

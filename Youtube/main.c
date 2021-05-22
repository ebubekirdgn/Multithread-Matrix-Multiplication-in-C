#include <stdio.h>
#include <pthread.h>
#include<time.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int thread_number;
int mat1[4096][4096],mat2[4096][4096],res[4096][4096];
int m,n;
void matrix_read()
{
	int i,j=0;
	printf("Matrix Length (exp.20x20) : ");
	scanf("%dx%d",&m,&n);
	
	printf("-----------------------------------------\n");
	
	printf("Matrix 1 : \n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			mat1[i][j] = rand() % 10 + 1;
			printf(" %2d ",mat1[i][j]);
		}
		printf("\n");
	}
	printf("-----------------------------------------\n");
	
	printf("Matrix 2 : \n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			mat2[i][j] = rand() % 10 + 1;
		    printf(" %2d ",mat2[i][j]);
		}
		printf("\n");
	}
	
}

void print_matrix(int xf[4096][4096])
{
	int i,j;
	printf("----------------------\n");
	for(i=0;i<m;i++)
	{
		printf("\n ");
		for(j=0;j<n;j++)
		{
			printf("  %d  ",xf[i][j]);
		}
		printf("\n");
	}
}



void *multiplication(void *arg)
{
	long int num = (long int)arg;
	int i,j,k=0;

	//Matrix Multiplication
	int from = (num*m)/thread_number;
	int to = (num+1)*m/thread_number;
	printf("\n From : %d To : %d ",from,to);
	
	for(i=from;i<to;i++)
	{
		for(j=0;j<n;j++)
		{
			//res[i][j]=0;
			
			for(k=0;k<n;k++)
			{
				res[i][j] += mat1[i][k] *mat2[k][j] ;
			}
		}
	} 
	
	printf("\n Matrix by thread %ld : \n ",num);
	printf("\n");
	print_matrix(res);
}



int main() {
	
    long int i,j;
	pthread_t tid[100];
		
	matrix_read();
	
	printf("\n----------------------\n");
	printf("Thread sayisi : ");
	scanf("%d",&thread_number);
	time_t start = time(NULL);
	
	//Creating Thread
	for(i=0;i<thread_number;i++){
		
		pthread_create(&tid[i],NULL,multiplication,(void*)i);
	}
	for(j=0;j<thread_number;j++){
		pthread_join(tid[j],NULL);
	}
	//Calculate Total Time Including 3 Soconds Sleep In Each Thread - - - -//
    
    printf(" ---> Time Elapsed : %.2f Sec\n\n", (double)(time(NULL) - start));
    
	 //Total Threads Used In Process - - - - - - - - - - - - - - - - - - - -//
    
    printf(" ---> Used Threads : %d \n\n",thread_number);
    int k=0;
	for(k=0;k<thread_number;k++)
        printf(" - Thread %d ID : %d\n",k+1,(int)tid[k]);
	
	return 0;
} 

#include<stdio.h>    
#include<stdlib.h>  
int main()
{  
int a[10][10],b[10][10],mul[10][10],r,c,i,j,k;
/* 
printf("enter the number of row = ");    
scanf("%d",&r);    
printf("enter the number of column = ");    
scanf("%d",&c);    
 */
 r = 10;
 c=10;
 	//Matrix 1 Random
	for(i=0;i<r;i++)    
	{    
		for(j=0;j<c;j++)    
		{    
		  a[i][j]=rand() % 10 + 1;
		}    
	}    
	
	char buffer[10];
    char filename[] = "a";
    sprintf(buffer, "%s%d.txt",filename,i);    
    FILE *dosya1 = fopen(buffer, "w");
	// Matrix 1  Printing
	printf("First Matrix \n");
	int f,g=0;
	for(f=0;f<i;f++){
        for(g=0;g<j;g++){
            printf("%2d ",a[f][g]);
            fprintf(dosya1, "%2d ",a[f][g]);
        }
        fprintf(dosya1,"\n"); 

    }
    fclose(dosya1);
		
	//Matrix 2 Random
	for(i=0;i<r;i++)    
	{    
		for(j=0;j<c;j++)    
		{    
			b[i][j] = rand() % 10 + 1;    
		}    
	}    
	
	char buffer2[10];
    char filename2[] = "b";
    sprintf(buffer2, "%s%d.txt",filename2,j);    
    FILE *dosya2 = fopen(buffer2, "w");
	
	printf("Second Matrix \n");
	int t,y=0;
	for(t=0;t<i;t++){
        for(y=0;y<j;y++){
            printf("%2d ",b[t][y]);
            fprintf(dosya2, "%d ",b[t][y]);
        }
        fprintf(dosya2,"\n"); 
    }
	fclose(dosya2);
 
printf("multiply of the matrix=\n");    
for(i=0;i<r;i++)    
{    
	for(j=0;j<c;j++)    
	{    
	mul[i][j]=0;    
		for(k=0;k<c;k++)    
		{    
		mul[i][j]+=a[i][k]*b[k][j];    
		}    
	}    
}   

char buffer3[10];
    char filename3[] = "result";
    sprintf(buffer3, "%s%d.txt",filename3,j);    
    FILE *result = fopen(buffer3, "w");
	
//for printing result    
	for(i=0;i<r;i++)    
	{    
		for(j=0;j<c;j++)    
		{    
			printf("%d\t",mul[i][j]); 
			fprintf(result, "%d ",mul[i][j]);   
		}    
		fprintf(result,"\n");    
	}    
	return 0;  
}  

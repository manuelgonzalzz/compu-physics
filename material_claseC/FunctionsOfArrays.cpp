# include <stdio.h>

# define SIZE 10
# define SIZE1 3
# define SIZE2 4

void FillVector(int v[SIZE]);
void FillMatrix(int m[SIZE1][SIZE2]);

int main()
{
	int i,j;
	int v[SIZE];
	int m[SIZE1][SIZE2];

	FillVector(v);
	printf("\n*****************\n");
	printf("vector v is:\n");

	for (i=0;i<SIZE;i++)
		printf("i=%i\t v[i]=%i\n", i, v[i]);

	FillMatrix(m);
	printf("\n\n*****************\n");
	printf("matrix m is:\n");

	for (i=0;i<SIZE1;i++)
	{
		for (j=0;j<SIZE2;j++)
			printf("%i\t",m[i][j]);
		printf("\n");
	}

	printf("\n");

	return 0;
}

void FillVector(int v[SIZE])
//almacena el vector v: v[i]=i*i
{
	int i;
	for(i=0;i<SIZE;i++)
		v[i]=i*i;

	return;
}


void FillMatrix(int m[SIZE1][SIZE2])
//almacena la matriz m: m[i][j]=i*j
{
	int i,j;
	for(i=0;i<SIZE1;i++)
		for(j=0;j<SIZE2;j++)
			m[i][j]=i*j;

	return;
}

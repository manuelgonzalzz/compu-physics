# include <stdio.h>

void OverWriteByValue(int i);
void OverWriteByReference(int& i);


int main()
{
	int i=10;

	printf("The initial value of i is %d\n\n",i);

	OverWriteByValue(i);
	printf("The value of i after overwriting by value is %d\n\n",i);
	
	OverWriteByReference(i);
	printf("The value of i after overwriting by reference is %d\n\n",i);

}

void OverWriteByValue(int i)
{
	i=22;
	printf("The value of i inside the function OverWriteByValue is %i\n",i);
}

void OverWriteByReference(int& i)
{
	i=44;
	printf("The value of i inside the function OverWriteByReference is %i\n",i);
}

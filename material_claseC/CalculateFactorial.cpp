# include <stdio.h>

int main()
{
    int num, factorial=1, i; //Hay que inicializar factorial=1
    printf("Enter an integer number:\n");
    scanf("%i",&num);
    for (i=1;i<=num;i++)
        factorial=factorial*i;

    printf("%i! = %i\n\n",num,factorial);

}

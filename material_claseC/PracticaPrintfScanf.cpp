# include <stdio.h>

////////////////////////////////////////////////////////////
// Programa para practicar con las funciones scanf y printf
//Probar distintos valores enteros y reales
////////////////////////////////////////////////////////////


int main()
{
	double x,y,z;
	int i,j,k;

	printf("Escribe un numero entero\n");
	scanf("%i",&i);

	printf(" i=%i\n",i);
	printf(" c=%c\n",i);

	printf("Escribe un numero real\n");
	scanf("%lf",&x);

	printf(" lf=%lf\n .3lf=%.3lf\n e=%e\n g=%g\n",x,x,x,x);

	return 0;
}

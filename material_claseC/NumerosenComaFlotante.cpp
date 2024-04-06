//Incluimos la biblioteca estandar
# include <stdio.h>

int main ()
{
	int i; //Variables contador
	double x,y; //Variables reales
	FILE *f1;

	//Inicializamos x a 2
	x=2.;

	for(i=0;i<1030;i++)
	{
		x=2.*x;
		printf("%d-th iteration:%lf=%e\n",i,x,x);//usar esto cuando x es double
		//printf("%d-th iteration:%d\n",i,x);//usar esto cuando x es int
	}
	//los numeros van a tener mas de 16 cifras significativas! pero deberiamos dudar de ellas a partir de la numero 17!
}


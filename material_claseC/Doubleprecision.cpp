#define _USE_MATH_DEFINES

# include <cstdio> //for printf
# include <iostream> //for cout
# include <cmath>  //for M_PI
# include <typeinfo> //for typeid

using namespace std;

int main(){
	double x = M_PI;

	printf("type of M_PI= %s\n",typeid(M_PI).name());

	printf("PI= %.26f = %e\n",x,x);

	printf("cos(PI/2)= %.26f = %e\n",cos(x/2),cos(x/2));

}

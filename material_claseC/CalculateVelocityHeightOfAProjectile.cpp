# include <stdio.h>
# include <math.h>

////////////////////////////////////////////////////////////////
// This program calculates the velocity and height of a projectile
// given its initial height, initial velocity, and constant
// acceleration. Variables used are:
// H0 : initial height (in m)
// H : height at any time (in m)
// V0 : initial vertical velocity (in m/s)
// V : vertical velocity at any time (in m/s)
// A : vertical acceleration (in m/s^2)
// T : time elapsed since projectile was launched (in s)
//
// Input : H0, V0, T
// Output : V, H
//
/////////////////////////////////////////////////////////////////


int main()
{
	int i,j,k;
	double x,y,z;

	double H0;	// initial height
	double H;	// height at any time
	double V0;	// initial vertical velocity
	double V;	// vertical velocity at any time
	double A;	// vertical acceleration
	double T;	// time elapsed since projectile was launched

	A = -9.807;
	printf("Enter the initial height (in meters):\n");
	scanf("%lf",&H0);

	printf("Enter the initial velocity (in meters per second):\n");
	scanf("%lf",&V0);

	printf("Enter time at which to calculate height and velocity (in seconds):\n");
	scanf("%lf",&T);

	H= 0.5*A*pow(T,2.)+V0*T+H0;
	V= A*T + V0;

	printf("At time %lf s the vertical velocity is %lf m/s\n",T,V);
	printf("and the height is %lf m\n",H);

	return 0;

}

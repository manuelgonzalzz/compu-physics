#include <stdio.h>
#include <math.h>
#include "complex.h" //include library to work with complex numbers. 
                     //REMEMBER: the file complex.h needs to be in the same directory as the current file!

int main()
{
    double x=3.;

    //declare complex numbers
    fcomplex a,b,c;

    //One way of defining the complex number is by assigning a value to its members (r for real and i for imaginary part)
    b.r=0.5;
    b.i=0.5;

    //Another way of defining the complex number is through the function Complex(real part, imaginary part)
    a=Complex(1.0,3.0);

    //complex number multiplication
    c=Cmul(a,b);

    //print the multiplication operation
    printf("\n( %.2lf + %.2lf i ) * ( %.2lf + %.2lf i ) = %.2lf + %.2lf i\n\n",a.r,a.i,b.r,b.i,c.r,c.i);

    //complex number addition
    c=Cadd(a,b);

    //print the addition operation
    printf("( %.2lf + %.2lf i ) + ( %.2lf + %.2lf i ) = %.2lf + %.2lf i\n\n",a.r,a.i,b.r,b.i,c.r,c.i);

    //Multiplication of a real number by a complex number
    c=RCmul(x,b);

    //print the multiplication operation
    printf("%.2lf*(%.2lf+%.2lf i)=%.2lf+%.2lf i\n\n",x,b.r,b.i,c.r,c.i);

    //abs
    x=Cabs(a);
    printf("abs( %.2lf + %.2lf i ) = %.2lf\n\n",a.r,a.i,x);

    //a*a^+
    c=Cmul(a,Conjg(a));
    printf("( %.2lf + %.2lf i ) * ( %.2lf + %.2lf i ) = %.2lf + %.2lf i\n\n",a.r,a.i,Conjg(a).r,Conjg(a).i,c.r,c.i);

}

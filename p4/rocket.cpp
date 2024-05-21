#include <stdio.h>
#include <cmath>

const double PI = 3.14159265359;
const double h=30;
const double N=1000;
const double w = 3.6617e-6;
const double dtl = 3.844e8;
const double Mt = 5.9736e24;
const double Ml = 0.07349e24;
const double G = 6.67e-11;
const double Rt = 6.378160e6;
const double Rl = 1.7374e6;
const double delta = G*Mt/(dtl*dtl*dtl);
const double mu = Ml/Mt;
void resetfile();

class rocket{
    public:
    //Las coordenadas del ejercicio: t, r(t), phi(t), p_r(t), p_phi(t)
        double r = Rt/dtl;
        double phi = PI/5;
        double p_r = (11200.0/dtl);
        double p_phi = 0.0;
        double t = 0.0;
    //Variables auxiliares para las funciones f(t=x0,x1,x2,x3,x4)    
        double x[5];
    //Parámetros Runge-Kutta
        double k1[4];
        double k2[4];
        double k3[4];
        double k4[4];

        


double f1(){return x[3];}
double f2(){return 1.*x[4]/(x[1]*x[1]);}
double f3(){
    double sumando1;
    double sumando2;
    sumando1 = (x[4]*x[4])/(x[1]*x[1]*x[1]);
    sumando2 = delta*(1./(x[1]*x[1]) + mu*(x[1]-cos(x[2]-w*x[0]))/(pow(1+x[1]*x[1]-2*x[1]*cos(x[2]-w*x[0]),3./2)));
    return sumando1 - sumando2;
}
double f4(){
    double factor1;
    double factor2;
    factor1 = (delta*mu*x[1])/((pow(1+x[1]*x[1]-2*x[1]*cos(x[2]-w*x[0]),3./2)));
    factor2 = sin(x[2]-w*x[0]);
    return -1.0*factor1*factor2;
}

void RK1(){
x[0] = t; x[1] = r; x[2] = phi; x[3] = p_r; x[4] = p_phi;
k1[0] = h*f1();
k1[1] = h*f2();
k1[2] = h*f3();
k1[3] = h*f4();
}
void RK2(){
    RK1();
    x[0] = t + 0.5*h;
    x[1] = r + 0.5*k1[0];
    x[2] = phi + 0.5*k1[1]; 
    x[3] = p_r + 0.5*k1[2] ; 
    x[4] = p_phi + 0.5*k1[3];

    k2[0] = h*f1();
    k2[1] = h*f2();
    k2[2] = h*f3();
    k2[3] = h*f4();

}
void RK3(){
    RK2();
    x[0] = t + 0.5*h;
    x[1] = r + 0.5*k2[0];
    x[2] = phi + 0.5*k2[1]; 
    x[3] = p_r + 0.5*k2[2] ; 
    x[4] = p_phi + 0.5*k2[3];
    
    k3[0] = h*f1();
    k3[1] = h*f2();
    k3[2] = h*f3();
    k3[3] = h*f4();
}
void RK4(){
    RK3();
    x[0] = t + h;
    x[1] = r + k2[0];
    x[2] = phi + k2[1]; 
    x[3] = p_r + k2[2] ; 
    x[4] = p_phi + k2[3];
}

void move(){

    FILE *fwrite;
    fwrite = fopen("rocket.txt","a");
    RK4();
    r = r + (1./6)*(k1[0]+2*k2[0]+2*k3[0]+k4[0]);
    phi = phi + (1./6)*(k1[1]+2*k2[1]+2*k3[1]+k4[1]);
    p_r = p_r + (1./6)*(k1[2]+2*k2[2]+2*k3[2]+k4[2]);
    p_phi = p_phi + (1./6)*(k1[3]+2*k2[3]+2*k3[3]+k4[3]);
    
    fprintf(fwrite,"%lf, ",r*cos(phi));
    fprintf(fwrite,"%lf\n",r*sin(phi));
    fprintf(fwrite,"%lf, ",cos(w*t));
    fprintf(fwrite,"%lf\n",sin(w*t));
    fprintf(fwrite,"\n");
    fclose(fwrite);
    
    t = t + h;

}

};

void resetfile(){
    FILE *fwrite;
    fwrite = fopen("rocket.txt","w");
    //fprintf(fwrite,"");
    fclose(fwrite);
}

int main(){
    resetfile();
    rocket apollo;
    for(int i=0;i<100000;i++){
        apollo.move();
    }
}
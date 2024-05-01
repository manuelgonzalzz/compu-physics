#include "complex.h"
#include <stdio.h>
#include <cmath>

const double h = 0.1; //Salto espacial
const int N = 1000; //Intervalo espacial
const double PI = 3.14159265358979;
const int ncicles = 250;
const double lambda = 0.75;

const fcomplex zero = Complex(0.0,0.0);
const fcomplex one = Complex(1.0,0.0);
const fcomplex minusone = Complex(-1.0,0.0);
void resetfile();

class wavefunction{

    public:
    fcomplex phi[N];
    fcomplex bound_cond1[N];
    fcomplex bound_cond2[N];
    double V_t[N] = {0.0}; //_t stands for tilde
    double k0_t;
    double s_t;
    double norm;

    double time = 0.0; //Volunteer exercise

    fcomplex Xi[N];
    fcomplex alpha[N];
    fcomplex beta[N];
    
    
void inicialize(){
    k0_t = 2.0*PI*ncicles/N;
    s_t = 0.25*k0_t*k0_t;
    for(int j=0; j<N; j++){
        if(j>0.4*N && j<0.6*N) V_t[j] = lambda*k0_t*k0_t;
        double rho;
        double arg;
        double re;
        double im;
        rho = exp(-8.0*(4*j-N)*(4*j-N)/(N*N));
        arg = k0_t*j;
        re = rho*cos(arg);
        im = rho*sin(arg);
        phi[j] = Complex(re,im);
    }
    phi[0] = Complex(0.0,0.0);
    phi[N-1] = Complex(0.0,0.0);

    alpha[N-1] = Complex(0.0,0.0);
    fcomplex gamma;
    fcomplex Aminus = one;
    fcomplex Aplus = one;
    fcomplex Azero;


    for(int j=N-1;j>0; j--){ //Calculate alpha

        Azero = Complex(-2-V_t[j],2.0/s_t);
        gamma = Cdiv(one, Cadd(Azero,Cmul(Aplus,alpha[j])));
        alpha[j-1] = Cmul(minusone,Cmul(Aminus,gamma));
    }

}

void calc_beta(){
    beta[N-1] = zero;
    fcomplex b;
    fcomplex gamma;
    fcomplex Azero;
    fcomplex Aminus = one;
    fcomplex Aplus = one;

    for(int j=N-1;j>0;j--){
        Azero = Complex(-2-V_t[j],2.0/s_t);
        gamma = Cdiv(one, Cadd(Azero,Cmul(Aplus,alpha[j])));
        b = Cmul(Complex(0.0,4.0/s_t),phi[j]);
        beta[j-1] = Cmul(gamma,
                        Csub(b,
                            Cmul(Aplus,beta[j])
                            )
                        );
        
    }
}

void calc_Xi(){
    Xi[0]=zero;
    for(int j=0;j<N-1;j++){
        Xi[j+1]=Cadd(
            Cmul(alpha[j],Xi[j]),
            beta[j]
        );
    }
}

void calc_phi(){
    fcomplex next_phi[N];
    for(int j=0;j<N;j++){
        next_phi[j] = Csub(Xi[j],phi[j]);
        phi[j] = next_phi[j];
    }
}

void print(){ //Print potential barrier, density of prob. and norm in different files.
    double probability;
    double norm = 0;
    FILE *fwrite;
    FILE *fnorm;
    fwrite = fopen("funciondeonda.txt","a");
    fnorm = fopen("norm.txt","a");
    for(int j=0;j<N;j++){
        probability = Cabs(phi[j])*Cabs(phi[j]);
        norm = norm + probability*h;
        fprintf(fwrite,"%lf, ",j*h);
        fprintf(fwrite,"%lf, ",probability);
        fprintf(fwrite,"%lf\n",V_t[j]);
    }
    fprintf(fwrite,"\n");
    fclose(fwrite);
    fprintf(fnorm,"%lf\n",norm);
    fclose(fnorm);
}

//Volunteer

int detect(){ //!!!!Preguntar la posicion del detector

    double prob_right = 0.0;
    double prob_left = 0.0;
    double prob_density;
    for(j=0;j<N/5;j++){ //No tiene sentido poner el detector en N/5
        prob_density = Cabs(phi[j])*Cabs(phi[j]);
        prob_left = prob_left + prob_density*h;
    } 
    for(j=N/5;j<N;j++){
        prob_density = Cabs(phi[j])*Cabs(phi[j]);
        prob_right = prob_right + prob_density*h;
    }
    return 0; 
}

};

void resetfile(){
FILE *fwrite;
FILE *fnorm;
fwrite = fopen("funciondeonda.txt","w");
fnorm = fopen("norm.txt","w");
fprintf(fwrite,"");
fprintf(fnorm,"");
fclose(fwrite);
fclose(fnorm);
}

int main(){
    resetfile();
    wavefunction wave;
    wave.inicialize();
    wave.print();
    for(int k=0;k<1000;k++){
        wave.calc_beta();
        wave.calc_Xi();
        wave.calc_phi();
        wave.print();
    }
}
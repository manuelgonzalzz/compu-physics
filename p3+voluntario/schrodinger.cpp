#include "complex.h"
#include <stdio.h>
#include <cmath>
#include <random>
#include <chrono>
#include <string>
#include <iostream>
using namespace std;

const int N = 1000; //Intervalo espacial
const double PI = 3.14159265358979;
const int ncicles = 250;
const fcomplex zero = Complex(0.0,0.0);
const fcomplex one = Complex(1.0,0.0);
const fcomplex minusone = Complex(-1.0,0.0);
void resetfile();

using namespace std;
unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
mt19937_64 generator(seed1);
uniform_real_distribution<double> r_distribution(0., 1.);

class wavefunction{

    public:
    fcomplex phi[N];
    fcomplex bound_cond1[N];
    fcomplex bound_cond2[N];
    double V_t[N] = {0.0}; //_t stands for tilde
    double k0_t = (2.0*PI*ncicles)/N;
    double s_t = 1.0/(4*k0_t*k0_t);


    double time = 0.0; //Voluntario exercise

    fcomplex Xi[N];
    fcomplex alpha[N];
    fcomplex beta[N];
    
    
void inicialize(double lambda){
    double rho;
    double arg;
    double re;
    double im;

    double integral = 0.0;
    for(int j=0;j<N;j++){
        integral = integral + exp(-8.0*(4*j-N)*(4*j-N)/(N*N))*exp(-8.0*(4*j-N)*(4*j-N)/(N*N));
    }

    for(int j=0; j<N; j++){
        if(j>0.4*N && j<0.6*N) V_t[j] = lambda*k0_t*k0_t;
        rho = exp(-8.0*(4*j-N)*(4*j-N)/(N*N))/sqrt(integral);
        arg = k0_t*j;
        re = rho*cos(arg);
        im = rho*sin(arg);
        phi[j] =Complex(re,im);
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
    double probability = 0.0;
    double norm = 0.0;
    FILE *fwrite;
    FILE *fnorm;
    fwrite = fopen("funciondeonda.txt","a");
    fnorm = fopen("norm.txt","a");
    for(int j=0;j<N;j++){
        probability = Cabs(phi[j])*Cabs(phi[j]);
        norm = norm + probability;
        fprintf(fwrite,"%i, ",j);
        fprintf(fwrite,"%e, ",probability*N);
        fprintf(fwrite,"%e\n",V_t[j]);
    }
    fprintf(fwrite,"\n");
    fclose(fwrite);
    fprintf(fnorm,"%lf\n",norm);
    fclose(fnorm);
}

void renormalize(){
    double integral = 0.0;
    for(int j=0;j<N;j++){
        integral = integral + Cabs(phi[j])*Cabs(phi[j]);
    }
    for(int j=0;j<N;j++){
        phi[j] = RCmul(1.0/sqrt(integral),phi[j]);
    }
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

/*Voluntario. Calculate Transmission Coeficcient*/

bool detect(wavefunction wave);
void showProgress(int progress, int total);

bool detect(wavefunction wave){
    double random;
    double prob_right = 0.0; //Probability that the particle tunnels through the barrier
    for(int j=2*N/5;j<N;j++){
        prob_right = prob_right + Cabs(wave.phi[j])*Cabs(wave.phi[j]);
    }
    random = r_distribution(generator);
    if(random<prob_right){ //Random event with probability prob_right
        return true;
    }
    else return false;
}

// Función para mostrar una barra de progreso
void showProgress(int progress, int total) {
    int barWidth = 70;
    float ratio = progress / (float)total;
    int c = ratio * barWidth;

    cout << "[";
    for (int x = 0; x < c; x++) cout << "=";
    for (int x = c; x < barWidth; x++) cout << " ";
    cout << "] " << int(ratio * 100.0) << " %\r";
    cout.flush();
}


int main(){
double lambda_list[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0}; //Lambda values to test
for(int i=0; i<10;i++){
    double lambda = lambda_list[i];
    string file_name = "lambda=" + to_string(lambda) + ".txt";
    resetfile();
    wavefunction wave;
    FILE *fdetect;
    fdetect = fopen(file_name.c_str(),"w");
    int transmission = 0;
    for(int Nexperiments=1; Nexperiments<=1000 ; Nexperiments++){
    wave.inicialize(lambda);
    bool seguir = true;
    int totaltime = 0;
    while(totaltime < 4000 && seguir){
        wave.calc_beta();
        wave.calc_Xi();
        wave.calc_phi();
        if(Nexperiments==1) wave.print(); //Only print first wave
        if(totaltime==2500){
            if(detect(wave)==true){ //If the particle is detected the loop ends
                seguir = false;
                transmission++;
            }
            else seguir = false;
            fprintf(fdetect,"%i, ",Nexperiments);
            fprintf(fdetect,"%i, ",transmission);
            fprintf(fdetect,"%lf\n", 1.0*transmission/Nexperiments);
        }
        totaltime++;
    }
    showProgress(Nexperiments, 1000);
    }
    fclose(fdetect);
}
}



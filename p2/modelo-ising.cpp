#include <stdio.h>
# include <iostream> //for cout
# include <cmath>  //for M_PI
# include <string>
# include <random>
#include <chrono>

#define N 32
using namespace std;
unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
mt19937_64 generator(seed1);
uniform_int_distribution<int> i_distribution(0, N-1);
uniform_real_distribution<double> r_distribution(0., 1.);



class red{
    public:
        int s[N][N];
        double T = 2.5;
        int nveces = 10000;
         
        void inicializar(bool random){
            if(random==false){
                for(int i=0;i<N;i++){
                    for(int j=0;j<N;j++){
                        s[i][j] = 1;
                    }
                }
            }
            if(random == true){
                double x;
                int k;
                unsigned seed1 = chrono::system_clock::now().time_since_epoch().count(); 
                mt19937_64 generator(seed1);
                
                uniform_int_distribution<int> i_distribution(0, 1); 
                for (int i=0;i<N;i++)
                {
                    for(int j=0;j<N;j++){
                        k=i_distribution(generator);
                        if(k==0){
                            s[i][j]=-1;
                        }
                        if(k==1){
                            s[i][j]=+1;
                        }
                    }

                }
            }
            if(random == false){
                for(int i=0;i<N;i++){
                    for(int j=0; j<N;j++){
                        s[i][j]=1;
                    }
                }
            }
        }
        void print(){
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    if(s[i][j] == +1) printf("+");
                    if(s[i][j]==-1) printf("-");
            }
            printf("\n");
            }
        }

        double probabilidad(int n, int m){
            double deltaE = 0.0;
            int n_inf;
            int n_sup;
            int m_inf;
            int m_sup;
            if(n==0) n_inf = N-1;
            else n_inf = n-1;
            if(n==N-1) n_sup = 0;
            else n_sup = n+1;
            if(m==0) m_inf = N-1;
            else m_inf = m-1;
            if(m==N-1) m_sup = 0;
            else m_sup = m+1;
            deltaE = 2.0*s[n][m]*(s[n_sup][m]+s[n_inf][m]+s[n][m_sup]+s[n][m_inf]);
            double p = exp(-deltaE/T);
            if(p<1) return p;
            if(p>1) return 1;
        }
        
};

void output(red r);
void pasomontecarlo(red &r);
void printpaso(red r);
double mag(red r);


int main(void){
    red r1;
    for(int t=0;t<10;t++){
        r1.T = 0.5 + 0.5*t;
        r1.inicializar(false);
        double mag_media = 0;
        for(int i=0;i<10000;i++){
            pasomontecarlo(r1);
            printpaso(r1);
            mag_media = mag_media + mag(r1);
        }
        mag_media = mag_media/10000.0;
        FILE *fmag;
        fmag = fopen("magnetizacion.txt","a");
        fprintf(fmag,"%lf,",r1.T);
        fprintf(fmag,"%lf\n",mag_media);
        }
}



void output(red r){
    string name="";
    name = name + "temp=" + to_string(int(10*r.T)) + ".txt";
    FILE *fwrite;
    fwrite = fopen(name.c_str(),"w");
    double mag;
    for(int k=0; k<r.nveces; k++){
        for(int l=0; l<N*N; l++){
            int n =i_distribution(generator);
            int m = i_distribution(generator);
            double Xi = r_distribution(generator);
            double p = r.probabilidad(n,m);
            if(Xi<p){
                r.s[n][m] = -1*r.s[n][m];
            }
        }
    int suma_s = 0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            suma_s = suma_s + r.s[i][j];
            if(j!=N-1) fprintf(fwrite,"%d,",r.s[i][j]);
            if(j==N-1) fprintf(fwrite,"%d",r.s[i][j]);
        }
        fprintf(fwrite,"\n");
    }
    mag = mag + (1.0/(N*N))*abs(suma_s);
    fprintf(fwrite,"\n");
    }
    mag = mag/r.nveces;
    cout << mag << endl;
    fclose(fwrite);
}
void pasomontecarlo(red &r){
        for(int l=0; l<N*N; l++){
            int n =i_distribution(generator);
            int m = i_distribution(generator);
            double Xi = r_distribution(generator);
            double p = r.probabilidad(n,m);
            if(Xi<p){
                r.s[n][m] = -1*r.s[n][m];
            }
        }
}
void printpaso(red r){
    FILE *fwrite;
    string name = "temp=" + to_string(int(10*r.T)) + ".txt";
    fwrite = fopen(name.c_str(),"a");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(j!=N-1) fprintf(fwrite,"%d,",r.s[i][j]);
            if(j==N-1) fprintf(fwrite,"%d",r.s[i][j]);
        }
        fprintf(fwrite,"\n");
    }
    fprintf(fwrite,"\n");
    fclose(fwrite);
}
double mag(red r){
    int suma_s = 0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            suma_s = suma_s + r.s[i][j];
        }
    }
    return (1.0/(N*N))*abs(suma_s);
}
#include <stdio.h>
# include <iostream> //for cout
# include <cmath>  //for M_PI
# include <string>

# define G 6.6743e-11
# define mass_sun 1.99e30
# define c 1.496e11
# define N 9
# define dim 2
# define h 0.1

using namespace std;

void transform_mass(double m[]);
void transform_pos(double r[][2]);
void transform_vel(double v[][2]);

void calculate_acc(double m[],double r[][2], double (&a)[N][2]);
void calculate_pos(double r[][2], double v[][2], double a[][2], double (&new_r)[N][2]);
void calculate_w(double v[][2], double a[][2], double (&w)[N][2]);
void calculate_vel(double w[][2],double new_a[][2],double (&new_v)[N][2]);
void calculate_ene(double m[],double r[][2], double v[][2],double E[]);
bool check_period(double r[],double rstart[]);


//Input:: masas planetas, posiciones iniciales r0[N][dim], velocidades iniciales v0[N][dim]
/* 
Todos los planetas comienzan en el eje X -- r0[i][0] = distancia planeta/sol; r0[i][1] = 0
Todas las velocidades comienzan en el eje Y -- v0[i][0] = 0; v0[i][0] = velocidad inicial.

*/
//Parametro:: incremento del tiempo -- pasos temporales
//Output:: posiciones de los plantetas r[N][dim], velocidades de los planetas v[N][dim]
int main(void){
    double m[N];
    double rstart[N][2];
    double r[N][2];
    double v[N][2];
    double a[N][2];
    double new_r[N][2];
    double w[N][2];
    double new_v[N][2];
    double new_a[N][2];
    double E[N];
    int num_vueltas[N]; //Periodos
    FILE *fwrite;
    FILE *fread;
    FILE *fenergy;
    fwrite = fopen("output.txt","w");
    fread = fopen("input.txt","r");
    fenergy = fopen("energia.txt","w");

    
    //Paso 0:: Inicializar y transformar las masas, posiciones y velocidades iniciales.
    int stop = 1;
    int i = 0;
    while (fscanf(fread,"%lf, %lf, %lf, %lf, %lf\n",&m[i],&r[i][0],&r[i][1],&v[i][0],&v[i][1])!=EOF && stop<100)
    {
        i++;
        stop++;
    }




    transform_mass(m); //Transformar
    transform_pos(r);
    transform_vel(v);
    
    for (int i=0;i<N;i++){ //Guardamos las posiciones iniciales para comprobar el periodo más adelante
        for(int j=0;j<2;j++){
            rstart[i][j] = r[i][j];
        }
        num_vueltas[i]=0;
    }

    
    calculate_acc(m,r,a); //Paso 1: evaluar a(t)
    for(int k=0; k<15000; k++){
        calculate_pos(r,v,a,new_r); //Paso 2: evaluar new_r == r(t+h)
        calculate_w(v,a,w); //Paso 2.5: evaluar w
        calculate_acc(m,new_r,new_a); //Paso 3: evaluar new_a == a(t+h)
        calculate_vel(w,new_a,new_v); //Paso 4: evaluar new_v
        for(int i=0;i<N;i++){ //Paso 5: Reinicializar las variables
            for(int j=0;j<dim;j++){
                r[i][j] = new_r[i][j];
                v[i][j] = new_v[i][j];
                a[i][j] = new_a[i][j];
            }
        }
        calculate_ene(m,r,v,E); //Paso 5.5: calcular energias 

        //Paso 6: Escribir las nuevas posiciones en el fichero
        for(int i=0;i<N;i++){
                fprintf(fwrite,"%lf, ", new_r[i][0]);
                fprintf(fwrite,"%lf\n", new_r[i][1]);
                fprintf(fenergy,"%.4e\n", E[i]);
        }
        fprintf(fwrite,"\n");
        fprintf(fenergy,"\n");
        //Paso 7: Calcular el periodo
        for(int i=1;i<N;i++){
            if(check_period(r[i],rstart[i])==true && num_vueltas[i]==0){
                num_vueltas[i] = k;
            }
        }
    }
    double T[N];
    for(int i=0;i<N;i++){

        T[i] = (num_vueltas[i]+1)*0.1*58.1;
        cout << to_string(T[i]) << endl;
    }

    fclose(fwrite);
    fclose(fenergy);
    return 0;
}

void inicializar(double m[], double r[][2], double v[][2]){
    FILE *fread;
    fread = fopen("input.txt","r");
}

void transform_mass(double m[]){
    double new_m[N];
    for(int i=0;i<N;i++){
        new_m[i] = 1e24*m[i]/(mass_sun);
        m[i] = new_m[i];
    }
}
void transform_pos(double r[][2]){
    double new_r[N][2];
    for(int i=0;i<N;i++){
        for(int j=0;j<dim;j++){
            new_r[i][j] = 1e9*r[i][j]/c;
            r[i][j] = new_r[i][j];
        }
    }

}

void transform_vel(double v[][2]){
    double new_v[N][2];
    for(int i=0;i<N;i++){
        for(int j=0;j<dim;j++){
            new_v[i][j] = 1e3*v[i][j]*pow(c/(G*mass_sun),0.5);
            v[i][j] = new_v[i][j];
        }
    }
}


void calculate_acc(double m[],double r[][2], double (&a)[N][2]){
    for(int i=0;i<N;i++)
    {
        a[i][0]=0.0;
        a[i][1]=0.0;
        for(int j=0;j<N;j++)
        {
                if(j!=i){
                    double modulo = (r[i][0]-r[j][0])*(r[i][0]-r[j][0])+(r[i][1]-r[j][1])*(r[i][1]-r[j][1]);
                    double dist = pow(modulo,3.0/2.0);
                    a[i][0]=a[i][0] - m[j]*(r[i][0]-r[j][0])/dist;
                    a[i][1]=a[i][1] - m[j]*(r[i][1]-r[j][1])/dist;
                }
        }
    }
}

void calculate_pos(double r[][2], double v[][2], double a[][2], double (&new_r)[N][2]){
    for(int i=0;i<N;i++){
        for(int j=0;j<dim;j++){
            new_r[i][j] = r[i][j] + h*v[i][j] + 0.5*h*h*a[i][j];
        }
    }
}

void calculate_w(double v[][2], double a[][2], double (&w)[N][2]){
    for(int i=0;i<N;i++){
        for(int j=0;j<dim;j++){
            w[i][j] = v[i][j] + 0.5*h*a[i][j];
        }
    }
}

void calculate_vel(double w[][2],double new_a[][2],double (&new_v)[N][2]){
    for(int i=0;i<N;i++){
        for(int j=0;j<dim;j++){
            new_v[i][j] = w[i][j] + 0.5*h*new_a[i][j];
        }
    }
}

void calculate_ene(double m[],double r[][2], double v[][2],double E[]){
    double T=0;
    double V=0;
    double dist;
    for(int i=0;i<N;i++){
        T = 0.5*m[i]*(v[i][0]*v[i][0]+v[i][1]*v[i][1]);
        T = T * mass_sun*mass_sun*G/c;
        V = 0.0;
        if(i!=0){
            dist = (r[i][0]-r[0][0])*(r[i][0]-r[0][0])+(r[i][1]-r[0][1])*(r[i][1]-r[0][1]);
            dist = sqrt(dist);
            V = -m[i]/dist;
            V = V*G*mass_sun*mass_sun/c;
        E[i] = T + V;
        }
    }
}

bool check_period(double r[],double rstart[]){
    double distx = fabs(100*(r[0]-rstart[0])/rstart[0]);
    double disty = r[1];
    if(distx<5 && disty<0 && disty > -0.15){
        return true;
    }
    else return false;
}
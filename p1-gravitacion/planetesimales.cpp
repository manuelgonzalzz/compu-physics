# include <stdio.h>
# include <iostream> //for cout
# include <cmath>  //for M_PI
# include <string>

const double G = 6.6743e-11;
const double mass_sun = 1.99e30;
const double c = 1.99e30;
const int N = 100;
const int dim = 2;
const double h = 0.1;

using namespace std;


class Planet
{
public:
    double mass;
    double radius;
    double x,y;
    double vx,vy;
    bool visible = true; //False si ha sido absorbido; True si no ha sido absorbido
    bool rock; //False si es gaseoso; True si es rocoso
    
};



//Funciones para inicializar
void inicializar(double m[],double r[][2],double v[][2]);
void transform_mass(double m[]);
void transform_pos(double r[][2]);
void transform_vel(double v[][2]);

//Algoritmo para el movimiento de los planetas
void move_planets(Planet planets[]);
void calculate_acc(double m[],double r[][2], double (&a)[N][2]);
void calculate_pos(double r[][2], double v[][2], double a[][2], double (&new_r)[N][2]);
void calculate_w(double v[][2], double a[][2], double (&w)[N][2]);
void calculate_vel(double w[][2],double new_a[][2],double (&new_v)[N][2]);
void calculate_ene(double m[],double r[][2], double v[][2],double E[]);
bool check_period(double r[],double rstart[]);

//Algoritmo para generar choque
void colision(Planet &p1, Planet &p2);

//Print posiciones de planetas
void printPlanets(Planet planets[]);





void ini_planets(Planet planets[]){ //Me he quedado x aqui.
    
    planets[0].mass = 1.0;
    planets[0].x = 0.0;
    planets[0].y = 0.0;
    planets[0].vx = 0.0;
    planets[0].vy = 0.0;
    planets[0].radius = 2.0;
    planets[0].rock = false; //El sol no interacctua en principio.

    FILE *fwrite;
    fwrite = fopen("radius.txt","w");
    fprintf(fwrite,"%lf\n",planets[0].radius);
    for(int i=1;i<N;i++){
        planets[i].mass = 0.01*(double)rand() / RAND_MAX;
        if(i<=N/10) planets[i].rock = true;
        else planets[i].rock = false;
        planets[i].x = 30*(-1+2*(double)rand() / RAND_MAX);
        planets[i].y = 30*(-1+2*(double)rand() / RAND_MAX);
        planets[i].vx = 10*(double)rand() / RAND_MAX;
        planets[i].vy = 10*(double)rand() / RAND_MAX;
        planets[i].visible = true;
        planets[i].radius = (double)rand() / RAND_MAX;
        fprintf(fwrite,"%lf\n",planets[i].radius);
        
    }
    fclose(fwrite);
}

void printPlanets(Planet planets[]){ //Hay alguna forma de hacer esto mas limpio??
        FILE *fwrite;
        fwrite = fopen("planetesimales.txt","a");
        for(int i=0;i<N;i++){
            fprintf(fwrite,"%lf, ",planets[i].x);
            fprintf(fwrite,"%lf\n", planets[i].y);
        }
        fprintf(fwrite,"\n");
    }

void move_planets(Planet planets[]){
        FILE* fwrite;
        fwrite = fopen("planetesimales.txt","a");
        double r[N][2],v[N][2],a[N][2],w[N][2];
        double new_r[N][2],new_v[N][2],new_a[N][2];
        double m[N];
        for(int i=0; i<N; i++){ //Reuso las funciones definidas en sistema_solar.cpp
            r[i][0] = planets[i].x;
            r[i][1] = planets[i].y;
            v[N][0] = planets[i].vx;
            v[N][1] = planets[i].vy;
            m[i] = planets[i].mass;
        }
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
        //Paso 6: Escribir las nuevas posiciones en el fichero
        for(int i=0;i<N;i++){
                if(planets[i].visible==true){
                fprintf(fwrite,"%lf, ", new_r[i][0]);
                fprintf(fwrite,"%lf\n", new_r[i][1]);
                }
                else{
                fprintf(fwrite,"%lf, ", new_r[i][0]);
                fprintf(fwrite,"%lf\n", new_r[i][1]);
                }
        }
        fprintf(fwrite,"\n");
    }

void colision(Planet &p1, Planet &p2){
        if(p1.rock == true && p2.rock == true && p1.visible==true && p2.visible==true){
            p2.visible = false;
            p1.radius = p1.radius*(pow((p1.mass + p2.mass)/p1.mass,1.0/3));
            p1.mass = p1.mass + p2.mass;
        }
}

void calculate_acc(double m[],double r[][2], double (&a)[N][2]){
    for(int i=0;i<N;i++)
    {
        a[i][0]=0.0;
        a[i][1]=0.0;
        if(i=!0){
            double modulo = (r[i][0]-r[0][0])*(r[i][0]-r[0][0])+(r[i][1]-r[0][1])*(r[i][1]-r[0][1]); //El sol en cero
            double dist = pow(modulo,3.0/2.0);
            a[i][0]= m[0]*(r[i][0]-r[0][0])/dist;
            a[i][1]= m[0]*(r[i][1]-r[0][1])/dist;
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


int main(){
    Planet planets[N];
    ini_planets(planets);
    printPlanets(planets);
    for(int i=1;i<10;i++){move_planets(planets);} //Tarda muchiiisimo
}


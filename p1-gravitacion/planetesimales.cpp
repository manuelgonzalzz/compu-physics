# include <stdio.h>
# include <iostream> //for cout
# include <cmath>  //for M_PI
# include <string>

const double G = 6.6743e-11;
const double mass_sun = 1.99e30;
const double c = 1.496e11;
const int N = 300;
const int dim = 2;
const double h = 0.1;
const double PI = 3.1415;

//Hay que definir las constantes que permiten pasar de SI a las unidades del problema SP
//Las constantes se definen como SP == alpha * SI

const double convert_mass = 5.025e-31; //inverse of sun mass
const double convert_dist = 6.684e-12; //inverse of c
const double convert_time = 1.992e-7; //in seconds


using namespace std;


class Planet
{
public:
    double mass;
    double radius;
    double x,y;
    double vx,vy;
    double ax, ay;
    double wx, wy;
    bool visible = true; //False si ha sido absorbido; True si no ha sido absorbido
    bool rock; //False si es gaseoso; True si es rocoso

};



//Funciones para inicializar
void ini_planets(Planet planets[]);

//Algoritmo para el movimiento de los planetas

void acceleration(Planet planets[]);
void positions(Planet planets[]);
void w(Planet planets[]);
void velocity(Planet planets[]);
void move(Planet planets[]);

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
    planets[0].ax = 0.0;
    planets[0].ay = 0.0;
    planets[0].wx = 0.0;
    planets[0].wy = 0.0;  

    planets[0].radius = 2.0;
    planets[0].rock = false; //El sol no interactua en principio.

    double omega; //angular velocity
    double rho; //polar coordinate
    double alpha; //angular coordinate
    FILE *fwrite;
    fwrite = fopen("radius.txt","w");
    fprintf(fwrite,"%.8e\n",planets[0].radius);
    for(int i=1;i<N;i++){
        
        //All planetesimals have mercury mass and radius (Neptune)
        planets[i].mass = 2.066e-5;
        planets[i].radius = 1.646e-4;
        
        if(i<N/10) planets[i].rock = true;
        else planets[i].rock = false;

        rho = 7.5+5*((double)rand() / RAND_MAX);
        alpha = 2*PI*((double)rand() / RAND_MAX);
        
        if((double)rand()/RAND_MAX < 0.5) omega = 0.1;
        else omega=-0.1;


        planets[i].x = rho*cos(alpha);
        planets[i].y = rho*sin(alpha);
        planets[i].vx = -omega*rho*sin(alpha) - 0.05*rho*cos(alpha);
        planets[i].vy = omega*rho*cos(alpha) - 0.05*rho*sin(alpha);//1.25*((double)rand() / RAND_MAX);
        planets[i].visible = true;
        planets[i].radius = 0.1*(double)rand() / RAND_MAX;
        fprintf(fwrite,"%.8e\n",planets[i].radius);
        
    }
    fclose(fwrite);
}

void printPlanets(Planet planets[]){ //Hay alguna forma de hacer esto mas limpio??
        FILE *fwrite;
        fwrite = fopen("planetesimales.txt","a");
        for(int i=0;i<N;i++){
            fprintf(fwrite,"%.8e, ",planets[i].x);
            fprintf(fwrite,"%.8e\n", planets[i].y);
        }
        fprintf(fwrite,"\n");
        fclose(fwrite);
    }

void colision(Planet &p1, Planet &p2){
    double distance;
    distance = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
    if(p1.visible==true && p2.visible==true && distance< (p1.radius + p2.radius)){ //Only visible planets can collide

        if(p1.rock == true && p2.rock == true){ //Rocky planets can always collide if they are close
            p2.visible = false;
            p2.x = 0.0;
            p2.y = 0.0;
            p1.vx = (p1.vx * p1.mass + p2.vx * p2.mass)/(p1.mass + p2.mass);
            p1.vy = (p1.vy * p1.mass + p2.vy * p2.mass)/(p1.mass + p2.mass);
            p1.radius = p1.radius*(pow((p1.mass + p2.mass)/p1.mass,1.0/3));
            p1.mass = p1.mass + p2.mass;
            p2.mass = 0.0;
            p2.radius = 0.0;

        }
        if(p1.rock == false && p1.rock == false){ //Gas planets only collide if out a certain distance
            double dist_asteroids = 2.10; //This value has to be determined better
            if((p1.x*p1.x + p1.y*p1.y)>dist_asteroids*dist_asteroids && (p2.x*p2.x + p2.y*p2.y)>dist_asteroids*dist_asteroids){
                p2.visible = false;
                p2.x = 0.0;
                p2.y = 0.0;
                p1.vx = (p1.vx * p1.mass + p2.vx * p2.mass)/(p1.mass + p2.mass);
                p1.vy = (p1.vy * p1.mass + p2.vy * p2.mass)/(p1.mass + p2.mass);
                p1.radius = p1.radius*(pow((p1.mass + p2.mass)/p1.mass,1.0/3));
                p1.mass = p1.mass + p2.mass;
                p2.mass = 0.0;
                p2.radius = 0.0;
            }

        }
    }
}

void acceleration(Planet planets[]){
    planets[0].ax = 0.0;
    planets[0].ay = 0.0;
    double dist_sun;

    for(int i=1;i<N;i++){
        if(planets[i].visible == true){
        dist_sun = (planets[i].x-planets[0].x)*(planets[i].x-planets[0].x) + (planets[i].y-planets[0].y)*(planets[i].y-planets[0].y);
        dist_sun = pow(dist_sun,3.0/2.0);
        planets[i].ax = -planets[0].mass*(planets[i].x-planets[0].x)/dist_sun;
        planets[i].ay = -planets[0].mass*(planets[i].y-planets[0].y)/dist_sun;
        }
    }
}

void positions(Planet planets[]){
    
    for(int i=0;i<N;i++){
        if(planets[i].visible == true){
        planets[i].x = planets[i].x + h*planets[i].vx + 0.5*h*h*planets[i].ax;
        planets[i].y = planets[i].y + h*planets[i].vy + 0.5*h*h*planets[i].ay;
        for(int j=0; j<N; j++){
            if(j!=i) colision(planets[i],planets[j]);
        }
    }
    }
}

void w(Planet planets[]){
    
    for(int i=0;i<N;i++){
        if(planets[i].visible == true){
        planets[i].wx = planets[i].vx + 0.5*h*planets[i].ax;
        planets[i].wy = planets[i].vy + 0.5*h*planets[i].ay;
    }
    }
}

void velocity(Planet planets[]){
    for(int i=0;i<N;i++){
        if(planets[i].visible == true){
        planets[i].vx = planets[i].wx + 0.5*planets[i].ax;
        planets[i].vy = planets[i].wy + 0.5*planets[i].ay;
    }
    }
}

void move(Planet planets[]){
    positions(planets);
    w(planets);
    acceleration(planets);
    velocity(planets);
}

int main(){
    FILE *fwrite;
    fwrite = fopen("planetesimales.txt","w");
    fprintf(fwrite,"");
    fclose(fwrite);

    Planet planets[N];
    ini_planets(planets);
    printPlanets(planets);
    acceleration(planets);
    for(int i=0; i<10000; i++){
        move(planets);
        printPlanets(planets);
    }
    for(int i=0;i<N;i++){
        printf("%lf\n",planets[i].radius);
    }
}
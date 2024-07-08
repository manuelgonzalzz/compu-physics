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
//Las constantes se definen como SP == theta * SI

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
    double internal = 0;
    int ncolissions = 0;
    double kinetic(){
        return 0.5*mass*(vx*vx+vy*vy);
    }
    double potential(){
        return -1.*mass/(sqrt(x*x+y*y));
    }
    double mecanic_energy(){
        return kinetic() + potential();
    }
    double excentricity(){
        double ang_momentum = mass*(x*vy-y*vx);
        return sqrt(1+
        (2.*mecanic_energy()*ang_momentum*ang_momentum/(mass*mass*mass))
        );
    }
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
    
    double r0=7.5;
    double delta=2./3;
    double omega = 0.1; //angular velocity
    double vr = -0.05;
    
    
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
 
    double rho; //polar coordinate
    double theta; //angular coordinate
    FILE *fwrite;
    fwrite = fopen("radius.txt","w");
    fprintf(fwrite,"%.8e\n",planets[0].radius);
    for(int i=1;i<N;i++){ 
        
        //All planetesimals have mercury mass and radius (Neptune)
        //planets[i].mass = 2.066e-5;
        //planets[i].radius = 1.646e-4;
        planets[i].mass = 2.858e-4;
        planets[i].radius = 3.893e-4;

        if(i<N/10) planets[i].rock = true;
        else planets[i].rock = false;

        //rho = 7.5+5*((double)rand() / RAND_MAX);
        rho = r0*(1+delta * (double)rand() / RAND_MAX);
        theta = 2*PI*((double)rand() / RAND_MAX);
        
        if((double)rand()/RAND_MAX < 0.5) omega = 1.0*omega;
        else omega=-1.0*omega;


        planets[i].x = rho*cos(theta);
        planets[i].y = rho*sin(theta);
        planets[i].vx = -omega*rho*sin(theta) +vr*rho*cos(theta);
        planets[i].vy = omega*rho*cos(theta) + vr*rho*sin(theta);//1.25*((double)rand() / RAND_MAX);
        planets[i].visible = true;
        //planets[i].radius = 9.15e-3;
        fprintf(fwrite,"%.8e\n",planets[i].radius);
        
    }
    fclose(fwrite);
}

void printPlanets(Planet planets[]){ //Hay alguna forma de hacer esto mas limpio??
        FILE *fwrite;
        fwrite = fopen("test1.txt","a");
        for(int i=0;i<N;i++){
            fprintf(fwrite,"%.8e, ",planets[i].x);
            fprintf(fwrite,"%.8e\n", planets[i].y);
        }
        fprintf(fwrite,"\n");
        fclose(fwrite);
    }

void colision(Planet &p1, Planet &p2){
    double distance;
    double initial_energy;
    double final_energy;
    distance = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
    if(p1.visible==true && p2.visible==true && distance< 50*(p1.radius + p2.radius)){ //Only visible planets can collide
    //50 is a factor that considers the attraction between close planetesimals
        if(p1.rock == true && p2.rock == true){ //Rocky planets can always collide if they are close
            initial_energy = p1.kinetic() + p2.kinetic();
            p2.visible = false;
            p2.x = 0.0;
            p2.y = 0.0;
            p1.vx = (p1.vx * p1.mass + p2.vx * p2.mass)/(p1.mass + p2.mass);
            p1.vy = (p1.vy * p1.mass + p2.vy * p2.mass)/(p1.mass + p2.mass);
            p1.radius = p1.radius*(pow((p1.mass + p2.mass)/p1.mass,1.0/3));
            p1.mass = p1.mass + p2.mass;
            p2.mass = 0.0;
            p2.radius = 0.0;
            final_energy = p1.kinetic();
            p1.internal = p1.internal + abs(final_energy-initial_energy);
            p1.ncolissions++;
            

        }
        if(p1.rock == false && p1.rock == false){ //Gas planets only collide if out a certain distance
            double dist_asteroids = 2.10; //This value has to be determined better
            if((p1.x*p1.x + p1.y*p1.y)>dist_asteroids*dist_asteroids && (p2.x*p2.x + p2.y*p2.y)>dist_asteroids*dist_asteroids){
                initial_energy = p1.kinetic() + p2.kinetic();
                p2.visible = false;
                p2.x = 0.0;
                p2.y = 0.0;
                p1.vx = (p1.vx * p1.mass + p2.vx * p2.mass)/(p1.mass + p2.mass);
                p1.vy = (p1.vy * p1.mass + p2.vy * p2.mass)/(p1.mass + p2.mass);
                p1.radius = p1.radius*(pow((p1.mass + p2.mass)/p1.mass,1.0/3));
                p1.mass = p1.mass + p2.mass;
                p2.mass = 0.0;
                p2.radius = 0.0;
                final_energy = p1.kinetic();
                p1.internal = p1.internal + abs(final_energy-initial_energy);
                p1.ncolissions++;
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
    FILE *foutput;
    fwrite = fopen("test1.txt","w");
    foutput = fopen("output.txt","w");
    fprintf(fwrite,"");
    fclose(fwrite);

    Planet planets[N];
    ini_planets(planets);
    printPlanets(planets);
    acceleration(planets);

    /*for(int i=0;i<N;i++){
        printf("%e\n",planets[i].mecanic_energy());
    }*/

    for(int i=0; i<10000; i++){
        move(planets);
        if(i>9000) printPlanets(planets);
    }

    fprintf(foutput,"index, radio, num_colisiones, energia_interna, excentricidad\n");
    int planet_index[N] = {0};
    for(int i=0;i<N;i++){
        if(planets[i].visible==true && abs(planets[i].x)<10 && abs(planets[i].y)<10 && planets[i].radius>1e-10  && planets[i].ncolissions>=0){
        planet_index[i]=i;
        fprintf(foutput,"%i, ",i);
        fprintf(foutput,"%.3e, ",planets[i].radius);
        fprintf(foutput,"%i, ",planets[i].ncolissions);
        fprintf(foutput,"%.3e, ",planets[i].internal);
        fprintf(foutput,"%.3e\n",planets[i].excentricity());
        }
    }
    for(int i=0; i<N; i++)
    {
        if(planet_index[i]!=0){
            int period=0;
            double x_media=0.0;
            double y_media=0.0;
            int k=2;
            double x_inicial = planets[i].x;
            double y_inicial = planets[i].y;
            move(planets);
            move(planets);
            while(abs((planets[i].x-x_inicial)/x_inicial)>0.015 && abs((planets[i].x-x_inicial)/x_inicial)>0.015 && k<1000){
                period++;
                x_media = x_media + planets[i].x;
                y_media = y_media + planets[i].y;
                k++;
                move(planets);
            }
            x_media = x_media/k;
            y_media = y_media/k;
            printf("%i, ",planet_index[i]);+
            printf("%lf, ", sqrt(x_media*x_media+y_media+y_media));
            printf("%i, ",period);
            printf("%.3e\n",planets[i].mass);
        }
    }

    fclose(foutput);
}

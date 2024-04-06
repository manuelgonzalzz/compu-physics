#include <stdio.h>
#define R 8.414
#define SIZE 10

void presion(double a, double b, double temperatura, double n, double vol_min, double vol_max);

int main()
{
    presion(0.0247,26.6e-6,273.,3.,0.5,1);
}

void presion(double a, double b, double temperatura, double n, double vol_min, double vol_max)
{
    FILE *fwrite;
    fwrite = fopen("output-vanderwaals.txt","w");
    fprintf(fwrite,"volumen, presion \n");
    double vol[SIZE], p[SIZE];
    double t = temperatura;
    for(int i=0;i<SIZE;i++)
    {
        vol[i] = vol_min + i*((vol_max-vol_min)/(SIZE-1));
        p[i] = (n*R*t)/(vol[i]-n*b)-(a*n*n)/(vol[i]*vol[i]);
        fprintf(fwrite,"%lf, ",vol[i]);
        fprintf(fwrite,"%lf \n",p[i]);

    }
    fclose(fwrite);
}
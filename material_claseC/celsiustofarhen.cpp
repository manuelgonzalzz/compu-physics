#include <iostream>
int main()
{
    double C,F; //C = Celsius; F = Farenheit
    int stop; //Me protejo de bucle infinito
    FILE *fread,*fwrite;
    fread = fopen("celsius.txt","r");
    fwrite = fopen("farenheit.txt","w");
    stop = 1;
    while (fscanf(fread,"%lf",&C)!=EOF && stop<100)
    {
        F =(9./5.)*C + 32.;
         
        stop++;
    }
    fclose(fread);
    fclose(fwrite);
    
}
//Hola 9/5C+32
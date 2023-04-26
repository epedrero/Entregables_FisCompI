#include <stdio.h>

int main(int argc, char *argv[]){
    //Inicializar y definir las variables
    int n=9;
    int a=1;
    int N=n*n; //Número totales de partículas, en este caso serían 9x9=81
    int pos=0; //Posición de la partícula
    int array[N][2]; //Array de Nx2

    //Recorrer el array agregando las posiciones de la partícula "pos" definiendo
    //sus posiciones en "x" (0), y en "y" (1)
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            array[pos][0] = i*a;
            array[pos][1] = j*a;
            pos++;
        }
    }

    //Imprimir linea por linea el array
    for(int i = 0; i < 81; i++) {
        printf("%d %d\n", array[i][0], array[i][1]);
    }

    return 0;
}
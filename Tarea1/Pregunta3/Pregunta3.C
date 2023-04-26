#include <stdio.h>
#include <stdlib.h>

//Primero se 
typedef double (*funcion)(double **, double **, int);

double *energia(double **posiciones, double **velocidades, int n_particulas){
    double *resultado = (double*)malloc(2 * sizeof(double*)); // reservamos memoria para dos valores de tipo double
    double K = 0, V = 0;
    //Definir output 
    resultado[0] = K;
    resultado[1] = V;
    return resultado;
}

struct Pregunta3
{
    int n_particulas; //Número de partículas
    double **posiciones;
    double **velocidades;
    funcion energia;
    double K; //Energía Cinética
    double V;//Energía Potencial
};

void init(struct Pregunta3 *sistema, int n_particulas) {
    sistema->n_particulas = n_particulas;

    // Reserva de memoria para las posiciones de las partículas
    //Reserva memoria para componentes x, y, z
    sistema->posiciones = (double**) malloc(3 * sizeof(double*));
    //Reserva memoria para la cantidad de partículas n_particulas
    for(int i = 0; i < 3; i++) {
        sistema->posiciones[i] = (double*) malloc(n_particulas * sizeof(double));
    }

    // Reserva de memoria para las velocidades de las partículas
    sistema->velocidades = (double**) malloc(3 * sizeof(double*));
    for(int i = 0; i < 3; i++) {
        sistema->velocidades[i] = (double*) malloc(n_particulas * sizeof(double));
    }
}

// Función para realizar cálculos de energía en el sistema de partículas
void calculos(Pregunta3 *sistema, double **posiciones, double **velocidades, int n_particulas, funcion energia) {
    // Llamada a la función externa que realiza los cálculos de energía
    double *resultados = energia(posiciones, velocidades, n_particulas);
    sistema->K = resultados[0];
    sistema->V = resultados[1];
    sistema->energia(sistema->posiciones, sistema->velocidades, sistema->n_particulas);
    free(resultados);
}

// Función para liberar la memoria reservada por el sistema de partículas
void liberar(Pregunta3 *sistema) {
    // Liberación de memoria de las posiciones de las partículas
    for(int i = 0; i < 3; i++) {
        free(sistema->posiciones[i]);
    }
    free(sistema->posiciones);

    // Liberación de memoria de las velocidades de las partículas
    for(int i = 0; i < 3; i++) {
        free(sistema->velocidades[i]);
    }
    free(sistema->velocidades);
}
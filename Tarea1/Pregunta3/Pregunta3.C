#include <stdlib.h>

//Primero se define un puntero funcion
typedef void (*funcion)(double **, double **, int, double *, double *);

//Se define una función "energía" que internamente hace el cálculo de las
//energías cinética y potencial, T y V respectivamente. Solo para tener retorno se
//asigna valor de retorno arbitrario 0.0 para cada caso.
void funcion_energia(double **posiciones, double **velocidades, int n_particulas, double *T, double *V) {
    *T = 0.0;
    *V = 0.0;
}

//Se define la estructura pedida
struct Pregunta3
{
    int n_particulas; //Número de partículas
    double **posiciones;
    double **velocidades;
    funcion calculo_energia;
    double T; //Energía Cinética
    double V;//Energía Potencial
};

//Se reservan las memorias pedidas
void init(struct Pregunta3 *sistema, int n_particulas, funcion energia) {
    //Se definen los valores de n_particulas, T y V al elemento del mismo nombre dentro de la estructura
    sistema->n_particulas = n_particulas;
    sistema->T = 0;
    sistema->V = 0;
    //Se define la función "energia" a "calculo_energia" dentro de la estructura
    sistema->calculo_energia = energia;
   //Se reserva memoria para los punteros posiciones y velocidades
    sistema->posiciones = malloc(3 * sizeof(double *));
    sistema->velocidades = malloc(3 * sizeof(double *));
     //Reserva memoria para componentes x, y, z de las posiciones y las velocidades
    for(int i = 0; i < 3; i++) {
        sistema->posiciones[i] = malloc(n_particulas * sizeof(double));
        sistema->velocidades[i] = malloc(n_particulas * sizeof(double));
    }
}

// Función para realizar cálculos de energía en el sistema de partículas
void calculos(struct Pregunta3 *sistema) {
    // Llamada a la función externa que realiza los cálculos de energía
     sistema->calculo_energia(sistema->posiciones, sistema->velocidades, sistema->n_particulas, &(sistema->T), &(sistema->V));
}

// Función para liberar la memoria reservada por el sistema de partículas
void liberar(struct Pregunta3 *sistema) {
    // Liberación de memoria de las posiciones y velocidades de la i-esima fila
    for(int i = 0; i < 3; i++) {
        free(sistema->posiciones[i]);
        free(sistema->velocidades[i]);
    }
    //Liberación de memoria de los punteros posiciones y velocidades
    free(sistema->posiciones);
    free(sistema->velocidades);
}
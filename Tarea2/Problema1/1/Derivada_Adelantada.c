#include <stdlib.h>
#include "header11.h"

// Función para calcular la diferencia adelantada
void DiferenciaAdelantada(double* vector, double* diferencia, int N) {
    for (int i = 0; i < N - 1; i++) {
        diferencia[i] = vector[i + 1] - vector[i];
    }
}

// Función para calcular la Derivada Adelantada
void DerivadaAdelantada(double* f, double* derivada, double h, int N) {
    double* diferencia1 = (double*) malloc((N - 1) * sizeof(double));
    double* diferencia2 = (double*) malloc((N - 2) * sizeof(double));
    double* diferencia3 = (double*) malloc((N - 3) * sizeof(double));

    DiferenciaAdelantada(f, diferencia1, N);
    DiferenciaAdelantada(diferencia1, diferencia2, N - 1);
    DiferenciaAdelantada(diferencia2, diferencia3, N - 2);

    for (int i = 0; i < N - 3; i++) {
        derivada[i] = (1 / h) * (diferencia1[i] - (1 / 2.0) * diferencia2[i] + (1 / 3.0) * diferencia3[i]);
    }

    free(diferencia1);
    free(diferencia2);
    free(diferencia3);
}
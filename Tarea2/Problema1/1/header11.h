#ifndef TRAPECIO_H
#define TRAPECIO_H

#include <math.h>

// Declaración de la Derivada Adelantada
void DiferenciaAdelantada(double* vector, double* diferencia, int N);
void DerivadaAdelantada(double* f, double* derivada, double h, int N);

#endif
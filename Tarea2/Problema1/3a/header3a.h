#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <math.h>
#include <stdlib.h>
// Declaración de las funciones a utilizar
void DiferenciaAdelantada(double* vector, double* diferencia, int N);
void DerivadaAdelantada(double* f, double* derivada, double h, int N);
double posicionPiston(double theta);

#endif
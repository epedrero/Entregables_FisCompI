#ifndef TRAPECIO_H
#define TRAPECIO_H

#include <math.h>

// Declaración del método del trapecio para la integración
double metodoTrapecio(double a, double b, int n, double (*func)(double, double, double, double, double), double m, double k, double b_val, double mu);

#endif
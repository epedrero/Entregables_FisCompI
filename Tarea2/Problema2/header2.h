#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <sys/stat.h>

// Declaraciones de funciones a utilizar
int edo(double t, const double y[], double f[], void *params);
double solucion_analitica(double t);
void euler(double h, double t, double y[]);
void rk4(double h, double t, double y[]);
void rk45(double h, double t, double y[]);

#endif
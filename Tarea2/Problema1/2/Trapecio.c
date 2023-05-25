#include "header2.h"

// Implementación del método del trapecio para la integración
double metodoTrapecio(double x0, double xn, int n, double (*func)(double, double, double, double, double), double m, double k, double b, double mu)  {
    double h = (xn - x0) / n; // Ancho
    double suma = ((*func)(x0, m, k, b, mu) + (*func)(xn, m, k, b, mu)) / 2.0; // Suma inicial

    for (int i = 1; i < n; i++) {
        double x = x0 + i * h;
        suma += (*func)(x, m, k, b, mu);
    }

    return h * suma;
}
#include "header2.h"

// Implementación del método del trapecio para la integración
double metodoTrapecio(double a, double b, int n, double (*func)(double, double, double, double, double), double m, double k, double b_val, double mu)  {
    double h = (b - a) / n; // Ancho
    double suma = ((*func)(a, m, k, b_val, mu) + (*func)(b, m, k, b_val, mu)) / 2.0; // Suma inicial

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        suma += (*func)(x, m, k, b_val, mu);
    }

    return h * suma;
}
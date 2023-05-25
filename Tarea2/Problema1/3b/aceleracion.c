#include "header3b.h"

// Definición función a utilizar
double aceleracion(double x, double m, double k, double b, double mu) {
    double g = -9.81; // Aceleración debido a la gravedad
    double aceleracion = -(mu * g + k / m * (mu * b + x) * (1 - b / sqrt(b * b + x * x)));
    return aceleracion;
}
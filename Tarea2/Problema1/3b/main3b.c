#include <stdio.h>
#include "header3b.h"
#include "header12.h"

int main() {
    // Parámetros de la función aceleración
    double m = 0.8; // Masa
    double b = 0.4; // Longitud natural
    double mu = 0.3; // Coeficiente de fricción
    double k = 80.0; // Constante de rigidez

    // Parámetros del método del trapecio
    double x_0 = b; // Condición inicial de posición
    double v_0 = 0.0; // Condición inicial de velocidad
    double x_n = 0.0; // Posición a encontrar
    double n = 1000; // Numero de divisiones
    double y_0 = v_0*v_0/2; // Velocidad inicial transformada

    // Calcular la integral de la aceleración utilizando el método del trapecio
    double y = y_0 + metodoTrapecio(x_0, x_n, n, &aceleracion, m, k, b, mu);
    double rapidez = sqrt(2*fabs(y));
    printf("La rapidez en x=0 es: %lf\n", rapidez);

    return 0;
}
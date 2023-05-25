#include <stdio.h>
#include "header3b.h"
#include "header2.h"

int main() {
    double m = 0.8; // Masa
    double b = 0.4; // Longitud natural
    double mu = 0.3; // Coeficiente de fricción
    double k = 80.0; // Constante de rigidez

    double a = b; // Límite inferior de integración
    double x_0 = b; // Condición inicial de posición
    double v_0 = 0.0; // Condición inicial de velocidad
    double n = 1000; // Numero de divisiones
    double y_0 = v_0*v_0/2; // Velocidad inicial transformada

    // Calcular la integral de la aceleración utilizando el método del trapecio
    double integral = y_0 + metodoTrapecio(a, x_0, n, &aceleracion, m, k, b, mu);
    double rapidez = sqrt(2*fabs(integral));
    printf("La rapidez en x=0 es: %lf\n", rapidez);

    return 0;
}
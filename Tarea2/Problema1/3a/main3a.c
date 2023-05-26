#include <stdio.h>
#include "header3a.h"
#include "header11.h"

int main() {
    double x1 = 0.0;
    double xN = 2 * M_PI;  // Ángulo completo (0 a 2π)
    int N = 1000;  // Cantidad de elementos
    double w = 523.6;

    double h = (xN - x1) / (N - 1);
    double* X = (double*) malloc(N * sizeof(double));
    double* f = (double*) malloc(N * sizeof(double));
    double* derivada = (double*) malloc((N - 3) * sizeof(double));

    // Rellenar el vector X con los valores correspondientes
    for (int i = 0; i < N; i++) {
        double theta_i = (2 * M_PI * i)/(N-1);
        X[i] = theta_i;
    }

    // Rellenar el vector f con los valores de la función func aplicada a cada elemento de X
    for (int i = 0; i < N; i++) {
        f[i] = posicionPiston(X[i]);
    }

    // Calcular el vector de velocidades
    DerivadaAdelantada(f, derivada, h, N);

    // Calcular el vector de aceleraciones
    double* aceleracion = (double*) malloc((N - 4) * sizeof(double));
    DerivadaAdelantada(derivada, aceleracion, h, N - 2);

    // Encontrar los máximos relativos de la aceleración
    int numMaximosRelativos = 0;
    double* maximosRelativos = (double*) malloc((N - 4) * sizeof(double));
    double* thetasMaximosRelativos = (double*) malloc((N - 4) * sizeof(double));

    for (int i = 1; i < N - 5; i++) {
        if (aceleracion[i] > aceleracion[i - 1] && aceleracion[i] > aceleracion[i + 1]) {
            maximosRelativos[numMaximosRelativos] = aceleracion[i];
            thetasMaximosRelativos[numMaximosRelativos] = X[i+1] * (180 / M_PI);  // El +1 compensa el desplazamiento en la derivada
            numMaximosRelativos++;
        }
    }

    // Guardar los datos en un archivo de texto
    FILE* archivo = fopen("datos.txt", "w");

    for (int i = 0; i < N - 3; i++) {
        double derivada_esc = derivada[i] * w;  // Escalar derivada por w

        fprintf(archivo, "%lf %lf\n", X[i], derivada_esc);
    }

    fclose(archivo);

    // Ejecutar el comando de Gnuplot para graficar los datos
    FILE* gnuplot = popen("gnuplot", "w");

    fprintf(gnuplot, "set term png\n");
    fprintf(gnuplot, "set output 'grafico.png'\n");
    fprintf(gnuplot, "set title 'v(theta) v/s theta'\n");
    fprintf(gnuplot, "unset key\n");
    fprintf(gnuplot, "plot 'datos.txt' using ($1 * 180 / pi):2 with lines\n");
    fprintf(gnuplot, "pause -1\n");

    pclose(gnuplot);

    // Mostrar los máximos relativos de la aceleración y los valores correspondientes de theta
    printf("Máximos relativos de la aceleración:\n");
    for (int i = 0; i < numMaximosRelativos; i++) {
        printf("Aceleración: %.2f, Theta: %.2f\n", maximosRelativos[i], thetasMaximosRelativos[i]);
    }

    // Liberar memoria
    free(X);
    free(f);
    free(derivada);
    free(aceleracion);
    free(maximosRelativos);
    free(thetasMaximosRelativos);

    return 0;
}
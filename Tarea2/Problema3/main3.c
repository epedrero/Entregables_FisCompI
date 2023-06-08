#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <math.h>

int funcion(double x, const double y[], double f[], void *params) {
    double *parametros = (double *)params;
    double k = parametros[2];
    double A = parametros[1];
    double P = parametros[0];
    double h_c = parametros[4];
    double T_inf = parametros[5];
    double s = parametros[3];

    f[0] = y[1];
    // Corregir esta funcion porque se despeja y''
    f[1] = (P / (A * k)) * (s * (pow(y[0], 4) - pow(T_inf, 4)) + h_c * (y[0] - T_inf));;

    return GSL_SUCCESS;
}

int solucion_pvi(int nf, double *x, double h, gsl_odeiv2_driver *d, double *Y) {
    double xi = 0.0;
    int status = 0;
    
    for (int n = 1; n <= nf; ++n) {
        xi = *x + h;
        status = gsl_odeiv2_driver_apply(d, x, xi, Y);
        
        if (status != GSL_SUCCESS) {
            printf("error, return value=%d\n", status);
            break;
        }
    }
    
    return 0;
}

int main() {
    double params[6] = {0.106, 2e-4, 120, 5.67e-8, 15, 293};
    double h = 0.001;
    double x0 = 0.0;
    double xf = 0.1;
    int nf = (xf - x0) / h;

    // Corregir params y darle el valor de los parámetros
    gsl_odeiv2_system sys = {funcion, NULL, 2, &params};
    gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0);
    
    double tol = 1e-9;
    double error = tol + 1;
    double y1_0 = 0.0;
    double y1_1 = 0.1;
    double y1_2 = 0.0;

    // Valores de frontera
    double T0 = 773.15; // T(0) = 773.15 K
    double dTdH = 273.15; // T'(H) = 273.15 K/m

    // Primera ejecución
    double x = 0.0;
    double Y[2] = {T0, 0};
    y1_0 = Y[1];
    solucion_pvi(nf, &x, h, d, Y);
    double error_0 = fabs(Y[1] - dTdH);

 // Segunda ejecución
    int nmax = 100;
    int n = 0;
    
    FILE *temperatura = fopen("temperatura.dat", "w");
    while (error > tol && n < nmax) {
        x = 0.0;
        Y[0] = T0;
        Y[1] = y1_1;
            
        solucion_pvi(nf, &x, h, d, Y);
        error = fabs(Y[1] - dTdH);

        y1_2 = y1_1 - (y1_1 - y1_0) / (error - error_0) * error;
        y1_0 = y1_1;
        y1_1 = y1_2;

        error_0 = error;
        ++n;
    }

    Y[0] = T0;
    Y[1] = y1_1;
    x = 0;
    for (int i = 1; i <= nf; i++) {
        double xi = x + h;
        int status = gsl_odeiv2_driver_apply(d, &x, xi, Y);
        fprintf(temperatura, "%.6f\t%.6f\n", xi, Y[0]);
        x = xi; // Actualizar el valor de x en cada iteración
    }

    fclose(temperatura);

    FILE *gnuplot = popen("gnuplot", "w");
    if (gnuplot) {
        fprintf(gnuplot, "set term png\n");
        fprintf(gnuplot, "set output 'temperatura.png'\n");
        fprintf(gnuplot, "set title 'Perfil de temperatura'\n");
        fprintf(gnuplot, "set xlabel 'x [m]'\n");
        fprintf(gnuplot, "set ylabel 'T [K]'\n");
        fprintf(gnuplot, "plot 'temperatura.dat' with lines\n");
        fprintf(gnuplot, "quit\n");
        pclose(gnuplot);
    }

    gsl_odeiv2_driver_free(d);
    remove("temperatura.dat");
    remove("ejecutable.out");

    return 0;
}

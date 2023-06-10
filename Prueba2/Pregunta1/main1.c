#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <math.h>

int ec_movimiento(double t, const double y[], double f[], void *params) {
    (void)(t);
    double *p = params;
    double G = p[1];
    double MT = p[2];

    f[0] = y[1]; // f_0 = dot(r)
    f[1] = y[0] * pow(y[3], 2)-(G * MT) / pow(y[0], 2); // f_1 = ddot(r)
    f[2] = y[3]; // f_2 = dot(theta)
    f[3] = - 2 * y[1] * y[3] / y[0]; // f_3 = ddot(theta)

    return GSL_SUCCESS;
}

int solucion_pvi(int nf, double *x, double h, gsl_odeiv2_driver *driver, double *Y) {
    double xi = 0.0;
    int status = 0;

    for (int n = 1; n <= nf; ++n) {
        xi = *x + h;
        status = gsl_odeiv2_driver_apply(driver, x, xi, Y);

        if (status != GSL_SUCCESS) {
            printf("error, return value=%d\n", status);
            break;
        }
    }

    return 0;
}

int main() {

    double H = 1072e3; //m
    double G = 6.674e-11; //Nm^2/kg^2
    double MT = 5.972e24; //kg
    double RT = 6371e3; //m
    double v0 = 7300; //m/s
    double phi = 15; //grados
    double D = 2000e3; //posicion objetivo

    double params[7] = {H, G, MT, RT, v0, phi, D};

    // condiciones iniciales
    double y_rk45[4] = {H + RT, v0 * sin(phi * M_PI / 180.0) , 0.0, v0 * cos(phi * M_PI / 180.0)/(H + RT)}; 
    double t = 0.0; // tiempo inicial
    double tf = 1000000.0; // tiempo final de simulación
    double dt = 1.0; // paso de tiempo

    FILE *data = fopen("trayectoria_misil.dat", "w"); // archivo para guardar los resultados

    // Método de Runge-Kutta-Fehlberg (RK45) para la solución de trayectoria
    // del misil
    gsl_odeiv2_system sys = {ec_movimiento, NULL, 4, &params};
    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rkf45, dt, 1, 0.0);

    double t_step = 0;
    double thetaf = 0;
    double rf = 0;
    while (t < tf) {
        t_step = t + dt;
        // Escribir los resultados en el archivo
        fprintf(data, "%.6g %lf %.6g %.6g %.6g\n",t,y_rk45[0],y_rk45[1],y_rk45[2],y_rk45[3]); 
        gsl_odeiv2_driver_apply(driver, &t, t_step, y_rk45);
        // Condición de altura y que vaya bajando
        if (fabs(y_rk45[0] - RT - D) <= 500 && y_rk45[1] < 0) {
            thetaf = y_rk45[2];
            rf = y_rk45[0];
            tf = t_step;
            break;
        }
        // Actualizar el tiempo
        t = t_step;
    }
    gsl_odeiv2_driver_free(driver);
    fclose(data);

    // Algoritmo para la trayectoria del misil

    t = 0.0; // Reiniciar el tiempo
    int N = 100;
    dt = (tf - t) / N; // Nuevo paso de tiempo basado en el número de pasos
    double tol = 1e-9;
    double error = tol + 1;
    double z1_0 = 0.0;
    double z1_1 = 0.1;
    double z1_2 = 0.0;

    // Corregir params y darle el valor de los parámetros
    gsl_odeiv2_system sysi = {ec_movimiento, NULL, 4, &params};
    gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, dt, 1, 0);

    // Valores de frontera
    double theta0 = M_PI;
    rf = rf;
    thetaf = thetaf;

    // Primera ejecución
    double x = 0.0;
    double Z[4] = {RT, 0.0, theta0, 0}; // condiciones iniciales del proyectil
    solucion_pvi(N, &x, dt, d, Z);
    double error_0 = fabs(Z[0] - rf);

    // Segunda ejecución
    int nmax = 100;
    int n = 0;

    FILE *impacto = fopen("proyectil.dat", "w");
    while (error > tol && n < nmax && fabs(Z[0] - rf) > tol && fabs(Z[2] - thetaf) > tol) {
        x = 0.0;
        Z[0] = RT;
        Z[1] = 0;
        Z[2] = theta0;
        Z[3] = z1_1;

        solucion_pvi(N, &x, dt, d, Z);
        error = fabs(Z[0] - rf);

        z1_2 = z1_1 - (z1_1 - z1_0) / (error - error_0) * error;
        z1_0 = z1_1;
        z1_1 = z1_2;

        error_0 = error;
        ++n;

        // Actualizar el ángulo theta0
        theta0 = Z[2];

        // Actualizar el ángulo thetaf si se ha alcanzado el radio deseado
        if (fabs(Z[0] - rf) <= tol) {
            thetaf = Z[2];
        }
    }

    Z[0] = RT;
    Z[1] = 0;
    Z[2] = theta0;
    Z[3] = z1_1;

    x = 0.0;
    for (int i = 1; i <= N; i++) {
        double xi = x + dt;
        int status = gsl_odeiv2_driver_apply(d, &x, xi, Z);
        // Invertir el sentido de theta
        double theta = 3 * M_PI / 2 - Z[2];
        fprintf(impacto, "%.6f %.6f %.6f %.6f %.6f\n", xi, Z[0], Z[1], theta, Z[3]);
        x = xi;
    }
    fclose(impacto);
    gsl_odeiv2_driver_free(d);


    FILE* gnuplot = popen("gnuplot -persist", "w");
        fprintf(gnuplot, "set terminal pngcairo enhanced size 1024, 1024\n");
        fprintf(gnuplot, "set output 'grafico.png'\n");
        fprintf(gnuplot, "set style data lines\n");
        fprintf(gnuplot, "set xlabel 'Eje x [m]'\n");
        fprintf(gnuplot, "set ylabel 'Eje y [m]'\n");
        fprintf(gnuplot, "set polar\n");
        fprintf(gnuplot, "set grid polar\n");
        fprintf(gnuplot, "plot 'trayectoria_misil.dat' using 4:2 ti 'Trayectoria Misil', 'proyectil.dat' using 4:2 ti 'Trayectoria proyectil', 6371000 ti 'Tierra'\n");
        pclose(gnuplot);

    remove("ejecutable.out");
    return 0;
}

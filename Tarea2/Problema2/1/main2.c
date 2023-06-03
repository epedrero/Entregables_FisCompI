#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

// Función para la ecuación diferencial
int edo(double t, const double y[], double f[], void *params)
{
    double m = 2.0;  // kg
    double c = 460.0;  // N · s/m
    double k = 450.0;  // N/m
    
    f[0] = y[1];
    f[1] = (-c / m) * y[1] - (k / m) * y[0];
    
    return GSL_SUCCESS;
}

// Solución analítica
double solucion_analitica(double t)
{
    double c = -0.0000873005;
    double term1 = exp((-115.0/2.0 - (5.0 * sqrt(511.0))/2.0) * t);
    double term2 = 115.547 * exp((-115.0/2.0 + (5.0 * sqrt(511.0))/2.0) * t);
    
    double x = c * (term1 - term2);
    
    return x;
}

// Método de Euler
void euler(double h, double t, double y[])
{
    double m = 2.0;  // kg
    double c = 460.0;  // N · s/m
    double k = 450.0;  // N/m
    
    double x = y[0];
    double v = y[1];
    
    double x_next = x + h * v;
    double v_next = v + h * ((-c / m) * v - (k / m) * x);
    
    y[0] = x_next;
    y[1] = v_next;
}

// Método de Runge-Kutta de orden 4 (RK4)
void rk4(double h, double t, double y[]){
    gsl_odeiv2_system sys = {edo, NULL, 2, NULL};
    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, h, 1e-6, 0.0);

    gsl_odeiv2_driver_apply(driver, &t, t + h, y);
    
    gsl_odeiv2_driver_free(driver);

}

// Método de Runge-Kutta-Fehlberg de quinto orden (RK45)
void rk45(double h, double t, double y[])
{
    gsl_odeiv2_system sys = {edo, NULL, 2, NULL};
    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rkf45, h, 1e-6, 0.0);
    
    gsl_odeiv2_driver_apply(driver, &t, t + h, y);
    
    gsl_odeiv2_driver_free(driver);
}

int main()
{
    double h1 = 0.005;  // Paso h1
    double h2 = 0.008;  // Paso h2
    double t = 0.0;  // Tiempo inicial
    double tf = 0.5;  // Tiempo final
    
    double y1[2] = {0.01, 0.0};  // Condiciones iniciales para h1
    double y2[2] = {0.01, 0.0};  // Condiciones iniciales para h2
    
    // Agregar .dat con puntos de solucion analítica posicion
    FILE *analitica = fopen("posicion_analitica.dat", "w");

    // Archivos .dat para guardar los datos de posición vs tiempo
    // utilizando el método de Euler y el método de RK4
    FILE *file1_euler = fopen("posicion_euler_h1.dat", "w");
    FILE *file2_euler = fopen("posicion_euler_h2.dat", "w");
    FILE *file1_rk4 = fopen("posicion_rk4_h1.dat", "w");
    FILE *file2_rk4 = fopen("posicion_rk4_h2.dat", "w");
    FILE *file_rk45 = fopen("posicion_rk45.dat", "w");
    
    // Archivos .dat para guardar los datos de error vs tiempo
    FILE *file_error_euler_h1 = fopen("error_euler_h1.dat", "w");
    FILE *file_error_euler_h2 = fopen("error_euler_h2.dat", "w");
    FILE *file_error_rk4_h1 = fopen("error_rk4_h1.dat", "w");
    FILE *file_error_rk4_h2 = fopen("error_rk4_h2.dat", "w");
    FILE *file_error_rk45 = fopen("error_rk45.dat", "w");
    
    // Archivos .dat para guardar los datos de velocidad vs tiempo
    FILE *file_v_euler_h1 = fopen("velocidad_euler_h1.dat", "w");
    FILE *file_v_euler_h2 = fopen("velocidad_euler_h2.dat", "w");
    FILE *file_v_rk4_h1 = fopen("velocidad_rk4_h1.dat", "w");
    FILE *file_v_rk4_h2 = fopen("velocidad_rk4_h2.dat", "w");
    FILE *file_v_rk45 = fopen("velocidad_rk45.dat", "w");
    
    // Método de Euler
    // Para h1
    while (t < tf)
    {
        double sol_analitica = solucion_analitica(t);
        double error_euler1 = fabs(y1[0] - sol_analitica);
        
        fprintf(file1_euler, "%.4f\t%.4f\n", t, y1[0]);
        fprintf(file_error_euler_h1, "%.100f\t%.100f\n", t, error_euler1);
        fprintf(file_v_euler_h1, "%.4f\t%.4f\n", t, y1[1]);
        
        euler(h1, t, y1);
        
        t += h1;
    }

    t=0.0; // Se restaura el tiempo

    // Para h2
    while (t < tf)
    {
        double sol_analitica = solucion_analitica(t);
        double error_euler2 = fabs(y2[0] - sol_analitica);
        
        fprintf(file2_euler, "%.4f\t%.4f\n", t, y2[0]);
        fprintf(file_error_euler_h2, "%.100f\t%.100f\n", t, error_euler2);
        fprintf(file_v_euler_h2, "%.4f\t%.4f\n", t, y2[1]);
        
        euler(h2, t, y2);
        
        t += h2;
    }
    
    // Método de RK4 utilizando la librería GSL
    t = 0.0;  // Reiniciar el tiempo
    double y_rk41[2] = {0.01, 0.0};  // Condiciones iniciales para h1
    double y_rk42[2] = {0.01, 0.0};  // Condiciones iniciales para h2
    
    // Para h1
    while (t < tf)
    {
        double sol_analitica = solucion_analitica(t);
        double error_rk41 = fabs(y_rk41[0] - sol_analitica);
        
        fprintf(file1_rk4, "%.4f\t%.4f\n", t, y_rk41[0]);
        fprintf(file_error_rk4_h1, "%.100f\t%.100f\n", t, error_rk41);
        fprintf(file_v_rk4_h1, "%.4f\t%.4f\n", t, y_rk41[1]);
        
        rk4(h1, t, y_rk41);

        t += h1;

    }

    t = 0.0; // Reiniciar tiempo
    // Para h2
    while (t < tf)
    {
        double sol_analitica = solucion_analitica(t);
        double error_rk42 = fabs(y_rk42[0] - sol_analitica);
        
        fprintf(file2_rk4, "%.4f\t%.4f\n", t, y_rk42[0]);
        fprintf(file_error_rk4_h2, "%.100f\t%.100f\n", t, error_rk42);
        fprintf(file_v_rk4_h2, "%.4f\t%.4f\n", t, y_rk42[1]);
        
        rk4(h2, t, y_rk42);

        t += h2;

    }
    
    // Método de RK45 utilizando la librería GSL
    t = 0.0;  // Reiniciar el tiempo
    double y_rk45[2] = {0.01, 0.0};  // Condiciones iniciales para RK45
    
    while (t < tf)
    {
        double sol_analitica = solucion_analitica(t);
        double error_rk45 = fabs(y_rk45[0] - sol_analitica);
        
        fprintf(file_rk45, "%.4f\t%.4f\n", t, y_rk45[0]);
        fprintf(file_error_rk45, "%.100f\t%.100f\n", t, error_rk45);
        fprintf(file_v_rk45, "%.4f\t%.4f\n", t, y_rk45[1]);
        
        rk45(h1, t, y_rk45);
        
        t += h1;
    }

    t = 0.0;
    // Agregar datos posicion analitica al .dat
    while (t < tf)
    {
        double sol_analitica = solucion_analitica(t);
        
        fprintf(analitica, "%.100f\t%.100f\n", t, sol_analitica);
        t += 1e-6;
    }
    
    // Cerrar archivos .dat
    fclose(analitica);
    fclose(file1_euler);
    fclose(file2_euler);
    fclose(file1_rk4);
    fclose(file2_rk4);
    fclose(file_error_euler_h1);
    fclose(file_error_euler_h2);
    fclose(file_error_rk4_h1);
    fclose(file_error_rk4_h2);
    fclose(file_error_rk45);
    fclose(file_rk45);
    fclose(file_v_euler_h1);
    fclose(file_v_euler_h2);
    fclose(file_v_rk4_h1);
    fclose(file_v_rk4_h2);
    fclose(file_v_rk45);
    
    // Gráfico de posición vs tiempo utilizando gnuplot
    FILE *gnuplot = popen("gnuplot", "w");
    if (gnuplot)
    {
        fprintf(gnuplot, "set term png\n");
        fprintf(gnuplot, "set output 'grafico_posicion.png'\n");
        fprintf(gnuplot, "set xlabel 'Tiempo'\n");
        fprintf(gnuplot, "set ylabel 'Posición'\n");
        fprintf(gnuplot, "set title 'Posición vs. Tiempo'\n");
        fprintf(gnuplot, "plot 'posicion_euler_h1.dat' with lines title 'Euler h=0.005', 'posicion_euler_h2.dat' with lines title 'Euler h=0.008', 'posicion_rk4_h1.dat' with lines title 'RK4 h=0.005', 'posicion_rk4_h2.dat' with lines title 'RK4 h=0.008', 'posicion_rk45.dat' with lines title 'RK45', 'posicion_analitica.dat' with lines title 'Real'\n");
        fprintf(gnuplot, "quit\n");
        
        pclose(gnuplot);
    }
    
    // Gráfico de error vs tiempo utilizando gnuplot
    FILE *gnuplot_error = popen("gnuplot", "w");
    if (gnuplot_error)
    {
        fprintf(gnuplot_error, "set term png\n");
        fprintf(gnuplot_error, "set output 'grafico_error.png'\n");
        fprintf(gnuplot_error, "set xlabel 'Tiempo'\n");
        fprintf(gnuplot_error, "set ylabel 'Error'\n");
        fprintf(gnuplot_error, "set title 'Error vs. Tiempo'\n");
        fprintf(gnuplot_error, "plot 'error_euler_h1.dat' with lines title 'Euler h=0.005', 'error_euler_h2.dat' with lines title 'Euler h=0.008', 'error_rk4_h1.dat' with lines title 'RK4 h=0.005', 'error_rk4_h2.dat' with lines title 'RK4 h=0.008', 'error_rk45.dat' with lines title 'RK45'\n");
        fprintf(gnuplot_error, "quit\n");
        
        pclose(gnuplot_error);
    }
    
    // Gráfico de velocidad vs tiempo utilizando gnuplot
    FILE *gnuplot_velocity = popen("gnuplot", "w");
    if (gnuplot_velocity)
    {
        fprintf(gnuplot_velocity, "set term png\n");
        fprintf(gnuplot_velocity, "set output 'grafico_velocidad.png'\n");
        fprintf(gnuplot_velocity, "set xlabel 'Tiempo'\n");
        fprintf(gnuplot_velocity, "set ylabel 'Velocidad'\n");
        fprintf(gnuplot_velocity, "set title 'Velocidad vs. Tiempo'\n");
        fprintf(gnuplot_velocity, "plot 'velocidad_euler_h1.dat' with lines title 'Euler h=0.005', 'velocidad_euler_h2.dat' with lines title 'Euler h=0.008', 'velocidad_rk4_h1.dat' with lines title 'RK4 h=0.005', 'velocidad_rk4_h2.dat' with lines title 'RK4 h=0.008', 'velocidad_rk45.dat' with lines title 'RK45'\n");
        fprintf(gnuplot_velocity, "quit\n");
        
        pclose(gnuplot_velocity);
    }
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

// Función para crear el vector f a partir de 
//aplicar la función f sobre el vector x. Tanto el 
//vector x como la función f se definen en el main
void apply_func(double *x, double *f, int N) {
    for (int i = 0; i < N; i++) {
        f[i] = f(x[i]);
    }
}

double Operador_derivada(double (*func)(double), double x, double h) {
    double delta_1 = Delta_mas(func, x, h); // primer término
    double delta_2 = Delta_mas(Delta_mas(func, x, h),x,h); // segundo término
    double delta_3 = Delta_mas(Delta_mas(Delta_mas(func, x, h),x,h), x, h); // tercer término
    double resultado = (1.0 / h) * (delta_1 - 0.5*delta_2 + (1.0/3.0)*delta_3); // calcular el operador Dz
    return resultado;
}
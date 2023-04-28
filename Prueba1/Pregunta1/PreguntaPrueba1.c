#include "headerP1.h"

int main()
{
    double k[5] = {1.0, 2.0, 1.0, 1.0, 2.0}; //Constantes elásticas  k1, k2, k3, k4, k5
    double w[3] = {2.0, 1.0, 2.0}; //Masas w1, w2, w3
    int signum = 1; // Almacena el signo del determinante

    //Se escribirán los vectores X, W y la matriz K tales que satisfacen
    //                                                K X = W
    //donde K es la matriz de coeficientes en términos de las constantes de elasticidades
    //k1, k2, k3, k4 y k5, X es el vector de desplazamientos x1, x2 y x3; y
    // W el vector de masas w1, w2 y w3.
    //Para esto, se utiliza la librería GSL

    // Define las matrices y vectores
    gsl_matrix *K = gsl_matrix_alloc(3, 3); // Matriz de coeficientes
    gsl_vector *X = gsl_vector_alloc(3); // Vector de desplazamientos x1, x2 y x3
    gsl_vector *W = gsl_vector_alloc(3); // Vector de masas w1, w2 y w3

    // Se llena la matriz K
    gsl_matrix_set(K, 0, 0, k[0]+k[1]+k[2]+k[4]);
    gsl_matrix_set(K, 0, 1, -k[2]);
    gsl_matrix_set(K, 0, 2, -k[4]);
    gsl_matrix_set(K, 1, 0, -k[2]);
    gsl_matrix_set(K, 1, 1, k[2]+k[3]);
    gsl_matrix_set(K, 1, 2, -k[3]);
    gsl_matrix_set(K, 2, 0, -k[4]);
    gsl_matrix_set(K, 2, 1, -k[3]);
    gsl_matrix_set(K, 2, 2, k[3]+k[4]);


  // Llenar el vector columna W
    gsl_vector_set(W, 0, w[0]);
    gsl_vector_set(W, 1, w[1]);
    gsl_vector_set(W, 2, w[2]);

    // Resolver el sistema de ecuaciones lineales K*x = W
    //Esta parte se obtuvo de 
    //https://www.gnu.org/software/gsl/doc/html/index.html
    gsl_permutation *p = gsl_permutation_alloc(3);
    gsl_linalg_LU_decomp(K, p, &signum);
    gsl_linalg_LU_solve(K, p, W, X);

    // Imprime el resultado
    printf("Los desplazamientos son: \n");
    printf("x1 = %f\n", gsl_vector_get(X, 0));
    printf("x2 = %f\n", gsl_vector_get(X, 1));
    printf("x3 = %f\n", gsl_vector_get(X, 2));

    // Liberar la memoria
    gsl_matrix_free(K);
    gsl_vector_free(W);
    gsl_vector_free(X);
    gsl_permutation_free(p);

    return 0;
}

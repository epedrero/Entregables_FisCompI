Se compila con:
gcc PreguntaPrueba1.c -lgsl -o PreguntaPrueba1
Luego se ejecuta el .out:
./PreguntaPrueba1

    gsl_matrix_set(K, 0, 0, k[0]+k[1]+k[2]+k[4]);
    gsl_matrix_set(K, 0, 1, -k[2]);
    gsl_matrix_set(K, 0, 2, -k[4]);
    gsl_matrix_set(K, 1, 0, -k[2]);
    gsl_matrix_set(K, 1, 1, k[2]+k[3]);
    gsl_matrix_set(K, 1, 2, -k[3]);
    gsl_matrix_set(K, 2, 0, -k[4]);
    gsl_matrix_set(K, 2, 1, -k[3]);
    gsl_matrix_set(K, 2, 2, k[3]+k[4]);
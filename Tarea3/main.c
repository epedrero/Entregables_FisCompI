#include <stdio.h>
#include <stdlib.h>
#include "dm.h"

int main(int argc, char *argv[]) {
    int N = 64;
    double tamanoCaja = 1.0;
    double diametro = 0.25;
    double radio = diametro / 2.0;

    Sistema *sistema = sistema_alloc(N);
    crear_fcc(sistema, tamanoCaja, diametro);
    guardar_datos(sistema, 4, "archivo.xyz", radio);
    sistema_free(sistema);
    remove("ejecutable.out");

    return 0;
}
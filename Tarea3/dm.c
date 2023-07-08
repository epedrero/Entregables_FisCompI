#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dm.h"

Sistema *sistema_alloc(int n) {
    Sistema *sistema = malloc(sizeof(Sistema));
    sistema->n = n;
    sistema->posiciones = malloc(n * sizeof(Vector3D));
    return sistema;
}

void sistema_free(Sistema *sistema) {
    free(sistema->posiciones);
    free(sistema);
}

void crear_fcc(Sistema *sistema, double tamanoCaja, double diametro) {
    int nc = 4; // número de celdas en cada dirección
    double constanteRed = tamanoCaja / nc;

    int indice = 0;
    for (int i = 0; i < nc; i++) {
        for (int j = 0; j < nc; j++) {
            for (int k = 0; k < nc; k++) {
                Vector3D posicion;
                posicion.x = i * constanteRed;
                posicion.y = j * constanteRed;
                posicion.z = k * constanteRed;
                sistema->posiciones[indice] = posicion;
                indice++;
                
                posicion.x = (i + 0.5) * constanteRed;
                posicion.y = (j + 0.5) * constanteRed;
                posicion.z = k * constanteRed;
                sistema->posiciones[indice] = posicion;
                indice++;
                
                posicion.x = (i + 0.5) * constanteRed;
                posicion.y = j * constanteRed;
                posicion.z = (k + 0.5) * constanteRed;
                sistema->posiciones[indice] = posicion;
                indice++;
                
                posicion.x = i * constanteRed;
                posicion.y = (j + 0.5) * constanteRed;
                posicion.z = (k + 0.5) * constanteRed;
                sistema->posiciones[indice] = posicion;
                indice++;
                
                if (indice == sistema->n) {
                    return;
                }
            }
            if (indice == sistema->n) {
                return;
            }
        }
        if (indice == sistema->n) {
            return;
        }
    }
}

void guardar_datos(Sistema *sistema, int nc, const char *nombreArchivo, double radio) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo: %s\n", nombreArchivo);
        return;
    }

    fprintf(archivo, "%d\n", sistema->n);
    fprintf(archivo, "Lattice=\"%d 0 0 0 %d 0 0 0 %d\"", nc, nc, nc);
    fprintf(archivo, " Properties=id:I:1:pos:R:3:radius:R:1\n");
    
    for (int i = 0; i < sistema->n; i++) {
        Vector3D posicion = sistema->posiciones[i];
        fprintf(archivo, "%d %f %f %f %f\n", i, posicion.x, posicion.y, posicion.z, radio);
    }

    fclose(archivo);
}

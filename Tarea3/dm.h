#ifndef DM_H_
#define DM_H_

typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

typedef struct {
    int n;
    Vector3D *posiciones;
} Sistema;

Sistema *sistema_alloc(int n);
void sistema_free(Sistema *sistema);
void crear_fcc(Sistema *sistema, double tamanoCaja, double diametro);
void guardar_datos(Sistema *sistema, int nc, const char *nombreArchivo, double radio);

#endif // DM_H_
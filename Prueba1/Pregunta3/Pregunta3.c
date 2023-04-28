#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Se define la cantidad de partículas
#define N 8

//Se define la estructura posiciones x, y, z 
typedef struct {
    double x, y, z;
} Vector;
//Se define la estructura de array posicion
typedef struct {
    Vector *pos;
} Particles;

//Inicialización de las partículas
void init_particles(Particles *p) {
    p->pos = (Vector *) malloc(N * sizeof(Vector));
    for (int i = 0; i < N; i++) {
        p->pos[i].x = (i % 2) * 1.0;
        p->pos[i].y = ((i >> 1) % 2) * 1.0;
        p->pos[i].z = ((i >> 2) % 2) * 1.0;
    }
}

//Función liberación de partículas
void free_particles(Particles *p) {
    free(p->pos);
}

//Función del potencial de Lennard-Jones
double lennard_jones_potential(double r) {
    double r6 = r * r * r * r * r * r;
    double r12 = r6 * r6;
    return 4.0 * (1.0 / r12 - 1.0 / r6);
}

//Función distancia entre dos puntos como vector
double distance(Vector v1, Vector v2) {
    double dx = v1.x - v2.x;
    double dy = v1.y - v2.y;
    double dz = v1.z - v2.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

//Total de energía potenciañ
double total_potential_energy(Particles *p) {
    double energy = 0.0;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            double dist = distance(p->pos[i], p->pos[j]);
            energy += lennard_jones_potential(dist);
        }
    }
    return energy;
}

//Main con los resultados
int main() {
    Particles particles;
    init_particles(&particles);
    double energy = total_potential_energy(&particles);
    printf("Total potential energy: %f\n", energy);
    free_particles(&particles);
    return 0;
}

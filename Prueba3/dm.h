#ifndef DM_H_
#define DM_H_

#include <stdio.h>
#define EPS 1.0
#define SIGMA 1

struct Sistema;
typedef struct Sistema sys;

sys * system_alloc(int, double, int, int);
void system_free(sys *);
void set_outputs(sys *, char* );
void crear_red_sc(sys *, double);
void crear_velocidades_gaussiana(sys *, double, int);
void guardar_datos(sys *);
void calcular_interacciones(sys *);
void medio_paso_verlet_velocity(sys *);
void fin_paso_verlet_velocity(sys *);
void calculo_energias(sys *);
void termodinamica(sys *);
void agregar_termostato(sys *, double, int, int, int);
void imprimir_info(sys *);
void simular(sys*, int);
void leer_datos(sys *, char*);

#endif // DM_H_

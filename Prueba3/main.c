#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dm.h"
#define NPASOS 1000
#define NFREQ_DUMP 20
#define NFREQ_INFO 100

void equilibrar(void);
void produccion(void);

int main(int argc, char *argv[]) {
  equilibrar();
  produccion();
  return 0;
}

void equilibrar(void){
  double densidad = 0.8;
  double temp_inicio = 2.0;
  double temp = 1.0;
  double semilla = 1234;
  int numero_particulas = 800;
  double dt = 0.005;
  int numero_pasos = 2000;
  int nfreq_dump = 2000;

  sys *sistema = system_alloc(numero_particulas, dt,
                              nfreq_dump, NFREQ_INFO);
  crear_red_sc(sistema, densidad);
  crear_velocidades_gaussiana(sistema, temp_inicio, semilla);

  set_outputs(sistema, "equilibrio.dat");
  agregar_termostato(sistema, temp, 500, 2000, 100);
  simular(sistema, numero_pasos);
  simular(sistema, NPASOS);
  system_free(sistema);
}

void produccion(void){
  int numero_particulas = 800;
  double dt = 0.005;
  sys *sistema = system_alloc(numero_particulas, dt,
                              NFREQ_DUMP, NFREQ_INFO);

  leer_datos(sistema, "equilibrio.dat");

  set_outputs(sistema, "produccion.dat");
  simular(sistema, NPASOS);


  system_free(sistema);
}
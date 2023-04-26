#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int maximo(int turnos[500]);
int minimo(int turnos[500]);
void imprimirHistograma(int *histograma, int num_cuadros, int min, int ancho_cuadros);

int main() {
    // Abrir archivo
    FILE *archivo = fopen("datos.dat", "r");

    // Debemos definir una variabla para alojar cada linea del archivo
    char string[1024];

    // Definimos para separar columnas de la linea
    char *token = NULL;
    // Adicionalmente, total lineas
    int num_lineas = 509-9;
    // Definir array para guardar informacion de los turnos
    int turnos[509-9];

    // leer primera linea
    fgets(string, 1024, archivo);
    for (int i=0; i<num_lineas; ++i) {
        fgets(string, 1024, archivo);
        token = strtok(string, " ");
        for (int j = 1; j<=8; ++j){
            token = strtok(NULL, " ");
            turnos[j] = atoi(token);
        }
    }
    fclose(archivo);

    // Queremos realizar analítica, calcular máximo y mínimo
    int max = maximo(turnos);
    int min = minimo(turnos);

    int ancho_cuadros = 2;
    int num_cuadros = (max - min +1)/ancho_cuadros;

    int *histograma = malloc(sizeof(int)* num_cuadros);
    for (int i=0; i<num_cuadros; ++i){
        histograma[i] = 0;
    }

    // Barras del histograma
    for (int i = 0; i < num_cuadros; ++i) {
    // Por cada linea del archivo.csv (turnos)
        for (int j=0; j < num_lineas; ++j) {
        // si el turno es igual a (min+i), le agregamos al histograma
            if (turnos[j] == min +i*ancho_cuadros){
                histograma[i]++;
            }
        }
    }
    imprimirHistograma(histograma, num_cuadros, min, ancho_cuadros);
    free(histograma);
    return 0;
}

int maximo(int turnos[500]){
  int max = turnos[0];
  for (int i=0; i<500 ; ++i) {
    if(max < turnos[i]){
      max = turnos[i];
    }
  }
  return max;
}

int minimo(int turnos[999]){
  int min = turnos[0];
  for (int i=0; i<999 ; ++i) {
    if(min > turnos[i]){
      min = turnos[i];
    }
  }
  return min;
}

void imprimirHistograma(int *histograma, int num_cuadros, int min, int ancho_cuadros){
  FILE *archivo = fopen("distancias.dat", "w");
  for (int i=0; i<num_cuadros; ++i) {
    // Imprimimos numero_turnos v/s frecuencia
    fprintf(archivo, "%d %d\n", i*ancho_cuadros+min, histograma[i]);
  }
  fclose(archivo);
}

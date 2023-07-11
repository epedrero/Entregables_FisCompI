#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dm.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

typedef struct {
  int paso; //paso actual de la simulacion
  double dt; // la cantidad de tiempo entre paso y paso
  double **fk; // auxiliar de fuerzas para verlet velocity
  double lbox; // largo de la caja
  double virial;
  FILE *fdatos; // archivo de datos
  int freq_dump;
  int freq_info;
  int termostat;
  int termostat_temp;
  int termostat_ini;
  int termostat_fin;
  int termostat_freq;
} auxiliar;

struct Sistema{
  int n; // numero de particulas
  double **r; // posicion de particulas
  double **v; // velocidad de particulas
  double **f; // fuerza de particulas
  double vol; // volumen del sistema
  double temp; // temperatura del sistema
  double press; // presión del sistema
  double utotal; // energía total del sistema
  double upot; // energía potencial del sistema
  double ukin; // energía cinetica del sistema
  auxiliar *aux; // estructura auxiliar
};

sys * system_alloc(int n, double dt, int freq_dump, int freq_info){
  sys * obj = malloc(sizeof(sys));

  obj->n = n;

  obj->r = malloc(sizeof(double *) * n);
  obj->v = malloc(sizeof(double *) * n);
  obj->f = malloc(sizeof(double *) * n);

  for (int i=0; i<n; ++i) {
    obj->r[i] = calloc(3, sizeof(double));
    obj->v[i] = calloc(3, sizeof(double));
    obj->f[i] = calloc(3, sizeof(double));
  }

  obj->temp = 0;
  obj->press = 0;
  obj->utotal = 0;
  obj->upot = 0;
  obj->ukin = 0;

  // Estructura auxiliar
  obj->aux = malloc(sizeof(auxiliar));
  obj->aux->fk = malloc(n*sizeof(double*));
  for (int i=0; i<n; ++i) {
    obj->aux->fk[i] = calloc(3, sizeof(double));
  }
  obj->aux->fdatos = NULL;// fopen("./datos.dat", "w");
  obj->aux->dt = dt;
  obj->aux->freq_info = freq_info;
  obj->aux->freq_dump = freq_dump;
  obj->aux->termostat = 0;
  return obj;
}

void system_free(sys *obj){
  for (int i = 0; i<(obj->n); i++) {
    free(obj->r[i]);
    free(obj->v[i]);
    free(obj->f[i]);
  }
  free(obj->r);
  free(obj->v);
  free(obj->f);

  // Estructura auxiliar
  for (int i = 0; i<(obj->n); i++) {
    free(obj->aux->fk[i]);
  }
  free(obj->aux->fk);
  fclose(obj->aux->fdatos);
  free(obj->aux);

  free(obj);
}

void set_outputs(sys *obj, char* archivo_dump){
  obj->aux->fdatos = fopen(archivo_dump, "w");
  if(!obj->aux->fdatos){
      puts("Error al abrir el archivo\n");
      exit(1);
    }
}

void crear_red_sc(sys *obj, double densidad){
  /* Funcion que crea una red cristalina cubica simple
   * se asume una caja que se puede particionar en
   * *nc* celdas en cada dimension
   */

  int l=0, m=0, n=0, contador = 0;
  int nc = ceil(pow((double)obj->n, 1.0/3.0)); //aproximar numero de celdas
  double lbox = pow((  (double)obj->n /densidad), 1.0/3.0);
  double a = lbox/nc;
  obj->aux->lbox = a*nc;
  obj->vol = pow(obj->aux->lbox, 3);

  double PF = ((double)obj->n)*4.0*M_PI/3.0*pow(1.0/2.0,3.0)/obj->vol;
  printf("Factor empaquetamiento %f\n", PF);

  for (l=0; l<nc; ++l) {
    for (m=0; m<nc; ++m) {
      for (n=0; n<nc; ++n) {
        obj->r[contador][0] = a*l;
        obj->r[contador][1] = a*m;
        obj->r[contador][2] = a*n;
        ++contador;
        if( contador==obj->n){
          break;
        }
      }
      if( contador==obj->n){
        break;
      }
    }
    if( contador==obj->n){
      break;
    }
  }

}

void crear_velocidades_gaussiana(sys *obj, double temp, int seed){
  gsl_rng * rg = gsl_rng_alloc(gsl_rng_minstd);
  gsl_rng_set(rg, seed);

  for (int i=0; i<(obj->n); ++i) {
    obj->v[i][0] = gsl_ran_gaussian(rg, sqrt(2 * temp));
    obj->v[i][1] = gsl_ran_gaussian(rg, sqrt(2 * temp));
    obj->v[i][2] = gsl_ran_gaussian(rg, sqrt(2 * temp));
  }
  
  gsl_rng_free(rg);
}

void guardar_datos(sys *obj){
  double L = obj->aux->lbox;
  FILE *archivo = obj->aux->fdatos;
  double step = obj->aux->paso;
  fprintf(archivo, "%i\n", obj->n);
  fprintf(archivo, "Lattice=\"%f 0 0 0 %f 0 0 0 %f\"", L, L, L);
  fprintf(archivo, " Properties=id:I:1:pos:R:3:velo:R:3:force:R:3:radius:R:1");
  fprintf(archivo, " Temp=%f PotentialEnergy=%f KineticEnergy=%f TotalEnergy=%f",
                     obj->temp, obj->upot, obj->ukin, obj->utotal);
  fprintf(archivo, " Time=%f\n", (double)step);
  for (int i = 0; i < obj->n; i++) {
    fprintf(archivo,
            "%i %f %f %f %f %f %f %f %f %f %f\n", i,
            obj->r[i][0],
            obj->r[i][1],
            obj->r[i][2],
            obj->v[i][0],
            obj->v[i][1],
            obj->v[i][2],
            obj->f[i][0],
            obj->f[i][1],
            obj->f[i][2],
            0.5);

  }
}

void leer_datos(sys *obj, char* f_input){
  int count = 0, id = 0;
  char * token;
  char lattice[50];
  char buffer[300];

  FILE *f_entrada = fopen(f_input, "r");

  while(fgets(buffer, sizeof(buffer), f_entrada)){
    if(count==0){
      obj->n = atoi(buffer);
    }else if(count==1){
      if (sscanf(buffer, "%*[^\"]\"%[^\"]\"", lattice) == 1) {
        obj->aux->lbox = atof(strtok(lattice, " "));
      }
      obj->vol = pow(obj->aux->lbox, 3);
    }else{
      token = strtok(buffer, " ");
      id = atoi(token);
      for (int i=0; i<3; ++i) {
        token = strtok(NULL, " ");
        obj->r[id][i] = atof(token);
      }
      for (int i=0; i<3; ++i) {
        token = strtok(NULL, " ");
        obj->v[id][i] = atof(token);
      }
    }
    count = count + 1;
  }
  fclose(f_entrada);
}

void imprimir_info(sys *obj){
  printf("%i T:%f P:%f kin:%f pot:%f tot:%f\n",
         obj->aux->paso, obj->temp, obj->press, obj->ukin, obj->upot, obj->utotal);
}

void medio_paso_verlet_velocity(sys *obj){
  double L = obj->aux->lbox;
  double dt = obj->aux->dt;
  for (int i=0;  i<(obj->n); ++i) {
    obj->r[i][0] = obj->r[i][0] + obj->v[i][0]*dt + 0.5*obj->f[i][0]*dt*dt;
    obj->r[i][1] = obj->r[i][1] + obj->v[i][1]*dt + 0.5*obj->f[i][1]*dt*dt;
    obj->r[i][2] = obj->r[i][2] + obj->v[i][2]*dt + 0.5*obj->f[i][2]*dt*dt;

    //condiciones periodicas de borde
    obj->r[i][0] = obj->r[i][0] - L*floor(obj->r[i][0]/L);
    obj->r[i][1] = obj->r[i][1] - L*floor(obj->r[i][1]/L);
    obj->r[i][2] = obj->r[i][2] - L*floor(obj->r[i][2]/L);
  }
}

void fin_paso_verlet_velocity(sys *obj){
  //double **fk = obj->aux->fk;
  double dt = obj->aux->dt;
  for (int i=0;  i<(obj->n); ++i) {
    obj->v[i][0] = obj->v[i][0] + 0.5*(obj->f[i][0] + obj->aux->fk[i][0])*dt;
    obj->v[i][1] = obj->v[i][1] + 0.5*(obj->f[i][1] + obj->aux->fk[i][1])*dt;
    obj->v[i][2] = obj->v[i][2] + 0.5*(obj->f[i][2] + obj->aux->fk[i][2])*dt;
  }
}

void calculo_energias(sys *obj){
  // Reset de energias
  obj->ukin = 0;
  obj->utotal = 0;

  for (int i=0; i<(obj->n); ++i ){
    for (int j=0; j<3 ; ++j) {
      obj->ukin = obj->ukin + (obj->v[i][j])*(obj->v[i][j]);
    }
  }
  obj->ukin = 0.5 * obj->ukin;
  obj->utotal = obj->ukin + obj->upot;
}

void termodinamica(sys *obj){
  // Temperatura instantanea
  obj->temp = (2.0/( 3.0* (obj->n - 1) ))*(obj->ukin);
  // Presion instantanea
  obj->press = ( (obj->n)/(obj->vol)*(obj->temp)
                 + (obj->aux->virial)/(obj->vol)
                );
}

void agregar_termostato(sys *obj, double temp, int t_ini, int t_fin, int freq){
  obj->aux->termostat = 1;
  obj->aux->termostat_temp = temp;
  obj->aux->termostat_ini = t_ini;
  obj->aux->termostat_fin = t_fin;
  obj->aux->termostat_freq = freq;
}

void aplicar_termostato(sys *obj){
  double lambda = sqrt( obj->aux->termostat_temp / obj->temp);

  for (int i=0; i<(obj->n); ++i) {
    obj->v[i][0] = obj->v[i][0]*lambda;
    obj->v[i][1] = obj->v[i][1]*lambda;
    obj->v[i][2] = obj->v[i][2]*lambda;
  }
}

void calcular_interacciones(sys *obj){

  double dx=0, dy=0, dz=0;
  double **r = obj->r;
  double dr2 = 0;
  double s2dr2 = 0, s2dr6 = 0, s2dr12 = 0;
  double sigma2 = SIGMA*SIGMA;
  double fij_aux = 0, fij_x = 0, fij_y = 0, fij_z=0;
  double Upot = 0;
  double rc2 = (2.5*SIGMA)*(2.5*SIGMA);
  double virial_tmp = 0;
  double L = obj->aux->lbox;

  for (int i = 0; i<(obj->n); ++i) {
    obj->f[i][0] = 0;
    obj->f[i][1] = 0;
    obj->f[i][2] = 0;
  }

  for (int i = 0; i< (obj->n)-1; ++i) {
    for (int j = i+1; j< (obj->n); ++j) {
      dx = r[j][0] - r[i][0];
      dy = r[j][1] - r[i][1];
      dz = r[j][2] - r[i][2];

      dx = dx - L*round(dx/L);
      dy = dy - L*round(dy/L);
      dz = dz - L*round(dz/L);

      dr2 = dx*dx + dy*dy + dz*dz;

      if(dr2<=rc2){
        s2dr2 = (sigma2)/dr2;
        s2dr6 = s2dr2*s2dr2*s2dr2;
        s2dr12 = s2dr6 * s2dr6;

        fij_aux = (s2dr12 - 0.5*s2dr6)/dr2;
        fij_x = fij_aux*dx;
        fij_y = fij_aux*dy;
        fij_z = fij_aux*dz;

        virial_tmp = virial_tmp + fij_aux*dr2;

        obj->f[i][0] = obj->f[i][0] - fij_x;
        obj->f[i][1] = obj->f[i][1] - fij_y;
        obj->f[i][2] = obj->f[i][2] - fij_z;

        obj->f[j][0] = obj->f[j][0] + fij_x;
        obj->f[j][1] = obj->f[j][1] + fij_y;
        obj->f[j][2] = obj->f[j][2] + fij_z;

        Upot = Upot + (s2dr12 - s2dr6);
      }
    }
  }

  obj->upot = 4*EPS*Upot;
  obj->aux->virial = virial_tmp*48/3*EPS;
  for (int i = 0; i < (obj->n); i++) {
    obj->f[i][0] = 48*EPS*obj->f[i][0];
    obj->f[i][1] = 48*EPS*obj->f[i][1];
    obj->f[i][2] = 48*EPS*obj->f[i][2];
  }
}

void corregir_vcm(sys *obj){
  double vtot[3]={0,0,0};
  for (int i=0; i<(obj->n) ; ++i) {
    for (int j=0; j<3; ++j) {
      vtot[j]=vtot[j]+obj->v[i][j];
    }
  }

  // Correccion
  for (int i=0; i<(obj->n) ; ++i) {
    for (int j=0; j<3; ++j) {
      obj->v[i][j] = obj->v[i][j] - vtot[j]/( (double)obj->n );
    }
  }
}

void simular(sys *obj, int npasos){
  if(obj->aux->fdatos == NULL){
    // No se ha inicializado fdatos
    obj->aux->fdatos = fopen("datos.dat", "w");
    if(!obj->aux->fdatos){
      puts("Error al abrir el archivo\n");
      exit(1);
    }
  }

  // Inicializacion, primeras interacciones
  corregir_vcm(obj);
  calcular_interacciones(obj);
  calculo_energias(obj);
  termodinamica(obj);

  for(int t=1; t<npasos+1; ++t){
    if ((t % obj->aux->freq_dump) == 0 ){
      guardar_datos(obj);
    }
    if ((t % obj->aux->freq_info) == 0 ){
      imprimir_info(obj);
    }
    //rk,   vk,   fk
    medio_paso_verlet_velocity(obj);
    //rk+1, vk,   fk
    for (int j=0; j<obj->n; ++j) {
      obj->aux->fk[j][0] = obj->f[j][0];
      obj->aux->fk[j][1] = obj->f[j][1];
      obj->aux->fk[j][2] = obj->f[j][2];
    }
    calcular_interacciones(obj);
    //rk+1, vk,   fk+1
    fin_paso_verlet_velocity(obj);
    //rk+1, vk+1, fk+1

    calculo_energias(obj);
    termodinamica(obj);
    obj->aux->paso = t;
    if(obj->aux->termostat==1){
      if( (t>=obj->aux->termostat_ini) &&  (t<=obj->aux->termostat_fin) ){
        if ((t-obj->aux->termostat_ini
             ) % (obj->aux->termostat_freq) == 0) {
          aplicar_termostato(obj);
        }
      }
    }
  }
    // Guardar las velocidades en un archivo
  FILE *fvelocidades = fopen("velocidades.dat", "w");
  for (int i = 0; i < obj->n; i++) {
    double vx = obj->v[i][0];
    double vy = obj->v[i][1];
    double vz = obj->v[i][2];
    double v = sqrt(vx * vx + vy * vy + vz * vz);
    fprintf(fvelocidades, "%f %f %f %f\n", vx, vy, vz, v);
  }
  fclose(fvelocidades);


// Generar la gráfica utilizando Gnuplot
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if (gnuplotPipe != NULL) {
    fprintf(gnuplotPipe, "set terminal png\n");
    fprintf(gnuplotPipe, "set output 'distribucion_vx.png'\n");
    fprintf(gnuplotPipe, "set title 'Distribución de Velocidades en el Eje X'\n");
    fprintf(gnuplotPipe, "set xlabel 'Distribución velocidad eje X'\n");
    fprintf(gnuplotPipe, "set ylabel 'Frecuencia Absoluta'\n");
    fprintf(gnuplotPipe, "set style histogram\n");
    fprintf(gnuplotPipe, "set style fill solid\n");
    fprintf(gnuplotPipe, "set boxwidth 0.1\n");
    fprintf(gnuplotPipe, "bin_width = 0.1\n");
    fprintf(gnuplotPipe, "bin(x, width) = width * floor(x / width)\n");
    fprintf(gnuplotPipe, "plot 'velocidades.dat' using (bin($1, bin_width)):(1) smooth frequency with boxes\n");
    pclose(gnuplotPipe);
  }

// Generar la gráfica para la velocidad en el eje Y utilizando Gnuplot
  FILE *gnuplotPipe_vy = popen("gnuplot -persistent", "w");
  if (gnuplotPipe_vy != NULL) {
    fprintf(gnuplotPipe_vy, "set terminal png\n");
    fprintf(gnuplotPipe_vy, "set output 'distribucion_vy.png'\n");
    fprintf(gnuplotPipe_vy, "set title 'Distribución de Velocidades en el Eje Y'\n");
    fprintf(gnuplotPipe_vy, "set xlabel 'Distribución velocidad eje Y'\n");
    fprintf(gnuplotPipe_vy, "set ylabel 'Frecuencia Absoluta'\n");
    fprintf(gnuplotPipe_vy, "set style histogram\n");
    fprintf(gnuplotPipe_vy, "set style fill solid\n");
    fprintf(gnuplotPipe_vy, "set boxwidth 0.1\n");
    fprintf(gnuplotPipe_vy, "bin_width = 0.1\n");
    fprintf(gnuplotPipe_vy, "bin(x, width) = width * floor(x / width)\n");
    fprintf(gnuplotPipe_vy, "plot 'velocidades.dat' using (bin($2, bin_width)):(1) smooth frequency with boxes\n");
    pclose(gnuplotPipe_vy);
  }

  // Generar la gráfica para la velocidad en el eje Z utilizando Gnuplot
  FILE *gnuplotPipe_vz = popen("gnuplot -persistent", "w");
  if (gnuplotPipe_vz != NULL) {
    fprintf(gnuplotPipe_vz, "set terminal png\n");
    fprintf(gnuplotPipe_vz, "set output 'distribucion_vz.png'\n");
    fprintf(gnuplotPipe_vz, "set title 'Distribución de Velocidades en el Eje Z'\n");
    fprintf(gnuplotPipe_vz, "set xlabel 'Distribución velocidad eje Z'\n");
    fprintf(gnuplotPipe_vz, "set ylabel 'Frecuencia Absoluta'\n");
    fprintf(gnuplotPipe_vz, "set style histogram\n");
    fprintf(gnuplotPipe_vz, "set style fill solid\n");
    fprintf(gnuplotPipe_vz, "set boxwidth 0.1\n");
    fprintf(gnuplotPipe_vz, "bin_width = 0.1\n");
    fprintf(gnuplotPipe_vz, "bin(x, width) = width * floor(x / width)\n");
    fprintf(gnuplotPipe_vz, "plot 'velocidades.dat' using (bin($3, bin_width)):(1) smooth frequency with boxes\n");
    pclose(gnuplotPipe_vz);
  }

  // Generar la gráfica para la magnitud de la velocidad utilizando Gnuplot
  FILE *gnuplotPipe_magnitud = popen("gnuplot -persistent", "w");
  if (gnuplotPipe_magnitud != NULL) {
    fprintf(gnuplotPipe_magnitud, "set terminal png\n");
    fprintf(gnuplotPipe_magnitud, "set output 'distribucion_magnitud.png'\n");
    fprintf(gnuplotPipe_magnitud, "set title 'Distribución de Magnitud de Velocidades'\n");
    fprintf(gnuplotPipe_magnitud, "set xlabel 'Magnitud de Velocidad'\n");
    fprintf(gnuplotPipe_magnitud, "set ylabel 'Frecuencia Absoluta'\n");
    fprintf(gnuplotPipe_magnitud, "set style histogram\n");
    fprintf(gnuplotPipe_magnitud, "set style fill solid\n");
    fprintf(gnuplotPipe_magnitud, "set boxwidth 0.1\n");
    fprintf(gnuplotPipe_magnitud, "bin_width = 0.1\n");
    fprintf(gnuplotPipe_magnitud, "bin(x, width) = width * floor(x / width)\n");
    fprintf(gnuplotPipe_magnitud, "plot 'velocidades.dat' using (bin($4, bin_width)):(1) smooth frequency with boxes\n");
    pclose(gnuplotPipe_magnitud);
  }


}

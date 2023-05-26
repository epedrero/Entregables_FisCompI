#include "header3a.h"

// Definición función a utilizar
double posicionPiston(double theta) {
    double R = 0.09;  // Largo manivela
    return R * (cos(theta) + sqrt(pow(2.5, 2) - pow(sin(theta), 2)));
}
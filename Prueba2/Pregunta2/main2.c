#include <stdio.h>
#include <math.h>

#define N 100
#define etha 1e-10
#define L 1.0

double V(double x) {
    if (x >= 0.0 && x <= 1.0) {
        return 50.0 * x/L;
    } else {
        return INFINITY;
    }
}

double phi(int n, double epsilon) {
    double h = L / N;
    
    double psi[N + 1];
    psi[0] = 0.0;
    psi[1] = 2.0 / N;
    
    for (int l = 1; l <= N - 1; l++) {
        double s_l = l * h;
        double v_c_l = V(s_l);
        
        psi[l + 1] = (2.0 * (1.0 - (5.0 / (6.0 * N * N)) * (epsilon - v_c_l)) * psi[l] -
                      (1.0 + (1.0 / (6.0 * N * N)) * (epsilon - V(s_l - h))) * psi[l - 1]) /
                      (1.0 + (1.0 / (6.0 * N * N)) * (epsilon - V(s_l + h)));
    }
    
    return psi[n];
}

double solve_schrodinger(int n) {
    double epsilon_a = 0.0;
    double epsilon_b = 45.0;
    double phi_a, phi_b, phi_c;
    
    do {
        phi_a = phi(n, epsilon_a);
        phi_b = phi(n, epsilon_b);
        
        if (phi_a * phi_b > 0) {
            epsilon_a = epsilon_b;
            epsilon_b = (n + 2) * (n + 2) * M_PI * M_PI / 2.0;
        } else {
            double epsilon_c = (epsilon_a + epsilon_b) / 2.0;
            phi_c = phi(n, epsilon_c);
            
            if (phi_a * phi_c < 0) {
                epsilon_b = epsilon_c;
            } else {
                epsilon_a = epsilon_c;
            }
        }
    } while (fabs(phi_a * phi_b) > etha && fabs(epsilon_b - epsilon_a) > etha);
    
    return epsilon_a;
}

int main() {
    
    FILE* eigenvalueData = fopen("eigenvalue.dat", "w");
    FILE* eigenfunctionData = fopen("eigenfunction.dat", "w");
    double eigenvalues[5];

    for (int n = 1; n <= 5; n++) {
        double energy = solve_schrodinger(n);
        fprintf(eigenvalueData, "%d %lf\n", n, energy);
        eigenvalues[n-1] = energy;
    }

    fprintf(eigenfunctionData, "x\t");
    for (int i = 0; i < 5; i++) {
        fprintf(eigenfunctionData, "eigenfunction%d\t", i + 1);
    }
    fprintf(eigenfunctionData, "\n");

    for (int k = 0; k <= N; k++) {
        double s = k * (L / N);
        fprintf(eigenfunctionData, "%lf\t", s); // Escribe el valor de 's' en la primera columna

        for (int i = 0; i < 5; i++) {
            double psi = phi(k, eigenvalues[i]);
            fprintf(eigenfunctionData, "%lf\t", psi); // Escribe el valor de 'psi' en la columna correspondiente
        }

        fprintf(eigenfunctionData, "\n");
    }

    fclose(eigenvalueData);
    fclose(eigenfunctionData);
    
    FILE* gnuplot = popen("gnuplot -persist", "w");
    fprintf(gnuplot, "set terminal pngcairo enhanced size 1024, 1024\n");
    fprintf(gnuplot, "set output 'grafico.png'\n");
    fprintf(gnuplot, "set style data lines\n");
    fprintf(gnuplot, "set xlabel 's'\n");
    fprintf(gnuplot, "set ylabel 'Eigenfunction'\n");
    fprintf(gnuplot, "plot 'eigenfunction.dat' using 1:2 ti 'Eigenfunction 1', \
            '' using 1:3 ti 'Eigenfunction 2', \
            '' using 1:4 ti 'Eigenfunction 3', \
            '' using 1:5 ti 'Eigenfunction 4', \
            '' using 1:6 ti 'Eigenfunction 5'\n");
    pclose(gnuplot);
    remove("ejecutable.out");
    
    return 0;
}

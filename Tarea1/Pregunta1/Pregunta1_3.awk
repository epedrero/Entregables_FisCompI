set terminal pngcairo enhanced size 640, 480
set output 'histograma.png'

set style data histogram
set style histogram rowstacked
set boxwidth 0.8 relative

set title "Histograma de distancias al origen"
set xlabel "Distancia"
set ylabel "Frecuencia"
set yrange [0:*]

plot 'distancias.dat' using 2:xtic(1) title "Distancias"
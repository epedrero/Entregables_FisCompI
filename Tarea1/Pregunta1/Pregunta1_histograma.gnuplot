set terminal pngcairo enhanced size 640, 640
set output 'histogram_distancias.png'

set style data lines

set xlabel 'Distancias al origen'
set ylabel 'Frecuencia'

binwidth=0.1
bin(x,width)=width*floor(x/width)

set title "Histograma de distancias al origen"
set xlabel "Distancias"
set xrange [0:500]
set ylabel "Frecuencia"
set grid

plot 'distancias.csv'
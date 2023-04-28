set terminal pngcairo enhanced size 640, 640
set output 'histogram_C.png'

set style data lines

set xlabel 'Factor beta de temperatura'
set ylabel 'Frecuencia'

binwidth=0.1
bin(x,width)=width*floor(x/width)

set title "Histograma de beta del elemento C"
set xlabel "Valores"
set xrange [0:380]
set ylabel "Frecuencia de turnos"
set grid

plot 'C_beta.csv'

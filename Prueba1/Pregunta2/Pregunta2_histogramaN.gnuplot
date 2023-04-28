set terminal pngcairo enhanced size 640, 640
set output 'histogram_N.png'

set style data lines



binwidth=0.1
bin(x,width)=width*floor(x/width)

set title "Histograma de beta del elemento N"
set xrange [0:110]
set xlabel 'Factor beta de temperatura'
set ylabel 'Frecuencia'
set grid

plot 'N_beta.csv'

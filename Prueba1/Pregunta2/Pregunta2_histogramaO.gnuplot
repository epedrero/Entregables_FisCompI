set terminal pngcairo enhanced size 640, 640
set output 'histogram_O.png'

set style data lines

set xlabel 'Factor beta de temperatura'
set ylabel 'Frecuencia'

binwidth=0.1
bin(x,width)=width*floor(x/width)

set title "Histograma de beta del elemento O"
set xlabel 'Factor beta de temperatura'
set ylabel 'Frecuencia'
set xrange [0:180]
set grid

plot 'O_beta.csv'
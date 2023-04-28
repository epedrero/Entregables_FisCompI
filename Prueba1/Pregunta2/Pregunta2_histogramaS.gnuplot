set terminal pngcairo enhanced size 640, 640
set output 'histogram_S.png'

set xlabel 'Factor beta de temperatura'
set ylabel 'Frecuencia'

binwidth=0.1
bin(x,width)=width*floor(x/width)

set title "Histograma de beta del elemento S"
set xlabel 'Factor beta de temperatura'
set ylabel 'Frecuencia'
set xrange [0:2]
set grid

plot 'S_beta.csv'
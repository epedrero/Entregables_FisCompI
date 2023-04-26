awk '{count[$2]++} END {print "N° Tipo 1:", count[1], "\nN° Tipo 2:", count[2]}' datos.dat

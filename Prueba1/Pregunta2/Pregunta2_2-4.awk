#!/usr/bin/awk -f 

BEGIN{
    FS=" "
    sum_x = 0;
    count_x = 0;
    cuadrado_x = 0;
    sum_y = 0;
    count_y = 0;
    cuadrado_y = 0;
    sum_z = 0;
    count_z = 0;
    cuadrado_z = 0;
    sum_beta = 0;
    count_beta = 0;
    cuadrado_beta = 0;
}
($7>0 && $1=="ATOM"){
    #Calcular valor de suma, contador y cuadrado para usarlo
    #en sqrt(V) = sqrt(E(X^2)-E^2(X))
    #Columna coordenada X
    sum_x = sum_x + $7;
    count_x = count_x+1;
    cuadrado_x = cuadrado_x + $7*$7;
}
($8>0 && $1=="ATOM"){
    #Columna coordenada Y
    sum_y = sum_y + $8;
    count_y = count_y+1;
    cuadrado_y = cuadrado_y + $8*$8;
}
($9>0 && $1=="ATOM"){
    #Columa coordenada Z
    sum_z = sum_z + $9;
    count_z = count_z+1;
    cuadrado_z = cuadrado_z + $9*$9;
}
($11>0 && $1=="ATOM"){
    #Columna beta
    sum_beta = sum_beta + $11;
    count_beta = count_beta+1;
    cuadrado_beta = cuadrado_beta + $11*$11;
}
END{
    print "El promedio de x es: " sum_x/count_x;
    print "La desviación estándar de x es: "sqrt(cuadrado_x/count_x - (sum_x/count_x)*(sum_x/count_x));
    print "El promedio de y es: "sum_y/count_y;
    print "La desviación estándar de y es: "sqrt(cuadrado_y/count_y - (sum_y/count_y)*(sum_y/count_y));
    print "El promedio de z es: " sum_z/count_z;
    print "La desviación estándar de z es: "sqrt(cuadrado_z/count_z - (sum_z/count_z)*(sum_z/count_z));
    print "El promedio de beta es: "sum_beta/count_beta;
    print "La desviación estándar de beta es: "sqrt(cuadrado_beta/count_beta - (sum_beta/count_beta)*(sum_beta/count_beta));
}
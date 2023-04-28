#!/usr/bin/awk -f 

BEGIN{
    #Definir separador. Consideró una columna extra, 
    #así que se considera eso en los cálculos (indexación de cols)
    FS=" "
    #Se inicializan las variables que se utilizarán
    count_C = 0;
    count_N = 0;
    count_O = 0;
    count_S = 0;
    #acá se utilizan los indices 0,1,2 para x,y,z respectivamente
    for (i = 0; i < 3; i++) {
        sum_C[i] = 0;
        sum_N[i] = 0;
        sum_O[i] = 0;
        sum_S[i] = 0;
        cuadrado_C[i] = 0;
        cuadrado_N[i] = 0;
        cuadrado_O[i] = 0;
        cuadrado_S[i] = 0;
    }
}
($12=="C"&& $1=="ATOM"){
    #Calcular valor de suma, contador y cuadrado para usarlo
    #en sqrt(V) = sqrt(E(X^2)-E^2(X))
    for(i=0;i<3;i++){
        sum_C[i] = sum_C[i] + $(i+7);
        cuadrado_C[i] = cuadrado_C[i] + $(i+7)*$(i+7);
    }
    count_C = count_C+1;
}
($12=="N"&& $1=="ATOM"){
    for(i=0;i<3;i++){
        sum_N[i] = sum_N[i] + $(i+7);
        cuadrado_N[i] = cuadrado_N[i] + $(i+7)*$(i+7);
    }
    count_N = count_N+1;
}
($12=="O"&& $1=="ATOM"){
    for(i=0;i<3;i++){
        sum_O[i] = sum_O[i] + $(i+7);
        cuadrado_O[i] = cuadrado_O[i] + $(i+7)*$(i+7);
    }
    count_O = count_O+1;
}
($12=="S"&& $1=="ATOM"){
    for(i=0;i<3;i++){
        sum_S[i] = sum_S[i] + $(i+7);
        cuadrado_S[i] = cuadrado_S[i] + $(i+7)*$(i+7);
    }
    count_S = count_S+1;
}
END{
    for(i=0;i<3;i++){
        print "El promedio de C" i " es: " sum_C[i]/count_C;
        print "La desviación estándar de C" i " es: "sqrt(cuadrado_C[i]/count_C - (sum_C[i]/count_C)*(sum_C[i]/count_C));
    }
    for(i=0;i<3;i++){
        print "El promedio de N" i " es: " sum_N[i]/count_N;
        print "La desviación estándar de N" i " es: "sqrt(cuadrado_N[i]/count_N - (sum_N[i]/count_N)*(sum_N[i]/count_N));
    }

    for(i=0;i<3;i++){
        print "El promedio de O" i " es: " sum_O[i]/count_O;
        print "La desviación estándar de O" i " es: "sqrt(cuadrado_O[i]/count_O - (sum_O[i]/count_O)*(sum_O[i]/count_O));
    }

    #Hay un solo atomo de S, por lo que las desv estandar son 0
    for(i=0;i<3;i++){
        print "El promedio de S" i " es: " sum_S[i]/count_S;
        print "La desviación estándar de S" i " es: "sqrt(cuadrado_S[i]/count_S - (sum_S[i]/count_S)*(sum_S[i]/count_S));
    }
}
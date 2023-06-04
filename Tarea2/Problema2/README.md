### Parte Teórica

Para resolver la ecuación diferencial que describe el movimiento de una masa adjunta a un resorte y un amortiguador

$$\ddot{x}(t)+\frac{c}{m}\dot{x}(t)+\frac{k}{m}x(t)=0$$

en donde
* $m=2$ kg
* $c=460$ Ns/m
* $k=450$ N/m

y las condiciones iniciales

* $x(0)=0.01$ m
* $\dot{x}(t)=0$ m/s

Se utilizaron los métodos de Euler para dos valores distinto de $h$, el método de Runge-Kutta de cuardo orden también para los mismos valores de $h$ y el método de Runge-Kutta-Fehlberg (RK45) con un $h$ inicial.\\

El archivo `main2.c` contiene tanto las definiciones de las funciones tales como `edo()`, `solucion_analitica()`, `euler()`, `rk4()` y `rk45()`, como también la implementación de éstos en el desarrollo del problema.

Para la solución de la EDO de manera análitica, se recurrió a `Mathematica`, el cual da la siguiente solución considerando las condiciones iniciales antes mencionadas

$$x(t)=-0.0000873005\left(e^{\left(-\frac{115}{2}-\frac{5\sqrt{511}}{2}\right)t}-115.547e^{\left(-\frac{115}{2}+\frac{5\sqrt{511}}{2}\right)t}\right)$$

### Instrucciones compilación y ejecución
Para la compilación y ejecución, debido a que hay un `Makefile`, se procede escribir `make` en la linea de comando, precurando encontrarse en el directorio de la pregunta dos llamado `Problema2`. Tras hacer el `make`, luego se escribe `make run` y se generará una carpeta llamada `Outputs` la cual contendrá:
1. 5 archivos `.dat` con las soluciones de las posiciones respectivas para cada método.
2. 5 archivos `.dat` con las soluciones para las velocidades utilizando cada método.
3. 3 gráficos, uno de las posiciones respecto al tiempo, en donde se incluye la posición real obtenida de la función analítica. Un gráfico con las velocidades respecto al tiempo y el último gráfico de los errores absolutos de las posiciones respecto a los valores reales.

### Observaciones y análisis de los resultados
En el `gráfico de Posición` $x(t)$ vs. tiempo $t$, se puede apreciar que las curvas estimadas por los métodos de Euler, Runge-Kutta de orden 4 (RK4) y Runge-Kutta-Fehlberg (RK45) todas graficadas para un número de pasos igual a 100, son todas muy similares a la curva analítica real.\\
Con respecto al `gráfico de velocidad`, casi todas las curvas tienen comportamientos parecidos, excepto para el método de Euler con $h_1=0.005$ y Euler con $h_2=0.008$, esto debido ser modelos no tan precisos como los otro y, además Euler $h_2$ tiene una menor iteraciones respecto a los otro, por lo que su precisión es menor.\\
Si observamos el `gráfico de error absoluto` de la posición $E(t)$ respecto al tiempo $t$, son levemente distinguibles para el primer $0.05 s$ las curvas para los métodos de $RK4$ con $h_1=0.005$ y $h_2=0.008$, y $RK45$. También es notorio las fluctuaciones del error para los primeros $0.2 s$ en el método de Euler con $h_2=0.008$. Esto no es de asombrar puesto que para el método de Euler con $h_1=0.005$ tiene una mayor cantidas de valores de división del intérvalo de tiempo $[0,0.5]$. A pesar de esto, todas las curvas adoptan la misma forma de curva para el error, tendiendo a errores menores a medida que pasa el tiempo.

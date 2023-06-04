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
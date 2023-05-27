# Parte 1
En la implementación de derivada se usó la expresión de la diferencia adelantada a tercer orden
$$D=\frac{1}{h}\left(\Delta_+-\frac{1}{2}\Delta_+^2+\frac{1}{3}\Delta_+^3\right)$$
Para esta implementación, dentro del mismo archivo `Derivada_Adelantada.c`, se definen las funciones `DiferenciaAdelantada` ($\Delta_+$) y `DerivadaAdelantada` ($D$). Se incluye en la misma carpeta el archivo `header11.h` el cual incluye las declaraciones de las dos funciones antes mencionadas y la librería `<math.h>`.

# Parte 2
Esta carpeta contiene el archivo `Trapecio.c` el cual contiene la definición de la función `metodoTrapecio` y el archivo `header12.h`, el cual contiene la declaración de la función de la implementación del método del trapecio y la librería `<math.h>`.

# Parte 3a
Esa pregunta tiene dos partes. En ese directorio (3a) se encuentran los archivos:
* `header3a.h`: en este se declaran las funciones `DiferenciaAdelantada`, `DerivadaAdelantada` y `posicionPiston`, junto a las librerías `<math.h>` y `<stdlib.h>`.
* `posicion.c`: se define la función ``posicionPiston`, la cual es la función que se da en el enunciado.
* `main3a.c`: en este archivo se definen los valores de los parámetros con los que se van a utilizar y los cálculos para obtener lo pedido en ambas partes.
* `Makefile`: este contiene las instrucciones para compilar los archivos y luego ejecutarlos.  

Para obtener los resultados solo hay que escribir en la terminal `make`, se generará un archivo `ejecutable.o` el cual se ejecuta en la terminal `./ejecutable.out` y se generarán los archivos correspondientes y se imprimirá los resultados en la terminal.

### Parte 3a.i
El gráfico de $v(\theta)$ vs. $\theta$ entre los ángulos $0^o$ y $360^o$, se obtuvo los datos de velocidad aplicando la función `DerivadaAdelantada`. El gráfico se obtiene tras la ejecución de `./ejecutable.out`, se genera un archivo llamado `grafico.png` dentro de la misma carpeta. 

### Parte 3a.ii
Para determinar numéricamente los valores de $\theta$ para los cuales la aceleración es máxima, primero se obtuvo los valores de aceleración, esto tras la aplicación de la función `DerivadaAdelantada` a los datos de velocidad. En un vector llamado `thetasMaximosRelativos` se van guardando los valores que cumplen que la aceleración es mayor que su vecino superior y que su vecino inferior. Los resultados se imprimen por la terminal.

# Parte 3b
### Parte Teórica
Dada la aceleración

$$\ddot{x}=-f(x)$$

se puede usar la propiedad

$$\frac{d}{dx}\left(\frac{\dot{x}^2}{2}\right)=\ddot{x}$$

Considerando el cambio de variable $y=\frac{\dot{x}^2}{2}$, entonces

$$\frac{d}{dx}y=-f(x)$$

$$\Rightarrow y=y_0-\int_{x_0}^{x}f(x')dx'$$

Por lo que se aplicará el método del Trapecio en términos de la variable nueva, $y$. Luego se realiza el cambio de variable $\dot{x}=\sqrt{2y}$ para encontrar la rapidez en $x=0$.

### Instrucciones Código
Para compilar, en la terminal integrada del directorio `fiscomp1_Entregables_EPedrero/Tarea2/Problema1/3b` hay que escribir `make`, esto compilará el Makefile, el cual generará un archivo `ejecutable.out`. Para ejecutarlo, en la terminal se escribe `./ejecutable.out`, en donde se mostrará el resultado en la misma terminal.
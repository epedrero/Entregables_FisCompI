# Pregunta 1

# Pregunta 2

# Pregunta 3a

# Pregunta 3b
### Parte Teórico
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
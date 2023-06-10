### Ejecución del código

Para ejecutar este código, desde la terminal del directorio de la `Pregunta 1` hay que escribir `make`, luego para ejecutar el archivo `ejecutable.out`, solo basta con escribir en la misma terminal `make run` y aparecerán tres archivos:

* `proyectil.dat`
* `trayectoria_misil.dat`
* `grafico.png`

### Observaciones

Es posible notar que la trayectoria del misil no coincide con la posición en la que el proyectil alcanza los $D = 2000$ kilómetros de altura en bajada, pero sí alcanza esta altura cuando el proyectil se encuentra en subida. Esto se debe a que el método utilizado para resolver el problema de valores de frontera (PVF) del misil, el `método de la secante` es sensible a los parámetros. Se intentó variando varios parámetros, incluso incluyendo la condición de estar a $r = RT + D$ junto a $\theta = \theta_f$, pero aún así la solución del método no era la que se quería.
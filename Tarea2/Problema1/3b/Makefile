build:
	gcc -Wall -Wextra -I../2 -c main3b.c -o main3b.o
	gcc -Wall -Wextra -c ../2/Trapecio.c -o Trapecio.o
	gcc -Wall -Wextra -c aceleracion.c -o aceleracion.o
	gcc -I../2 main3b.o Trapecio.o aceleracion.o -o ejecutable.out -lm

run:
	./ejecutable.out
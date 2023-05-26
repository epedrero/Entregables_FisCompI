build:
	gcc -Wall -Wextra -I../1 -c main3a.c -o main3a.o
	gcc -Wall -Wextra -c ../1/Derivada_Adelantada.c -o Derivada_Adelantada.o
	gcc -Wall -Wextra -c posicion.c -o posicion.o
	gcc -I../1 main3a.o Derivada_Adelantada.o posicion.o -o ejecutable.out -lm

run:
	./ejecutable.out
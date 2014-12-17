all: libLinalg.a

libLinalg.a : Linalg.o
	ar -cvq libLinalg.a Linalg.o
	ranlib libLinalg.a
	rm *.o

Linalg.x : Linalg.c
	cc -Wall Linalg.c -o Linalg.x -lm

clean :
	#rm *.x
	rm *.a

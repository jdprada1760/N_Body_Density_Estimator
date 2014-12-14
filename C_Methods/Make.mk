all: libLinalg.a

libLinalg.a : Linalg.x
	ar rc libLinalg.a Linalg.x
	ranlib libLinalg.a
	rm *.x

Linalg.x : Linalg.c
	cc -Wall Linalg.c -o Linalg.x -lm

clean :
	#rm *.x
	rm *.a

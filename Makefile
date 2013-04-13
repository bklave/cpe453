all: malloc.o malloc.a tryme

malloc.o:
	gcc -c malloc.c

malloc.a:
	ar r malloc.a malloc.o

tryme:
	gcc tryme.c malloc.a -o tryme.out

clean:
	rm malloc.a malloc.o tryme.out

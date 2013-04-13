all: malloc.o malloc.a tryme.o tryme

util.o:
	$(CC) $(CFLAGS) -c util.c

freelist.o: util.o
	$(CC) $(CFLAGS) -c freelist.c

malloc.o: util.o freelist.o
	$(CC) $(CFLAGS) -c malloc.c

malloc.a: malloc.o
	ar r malloc.a util.o freelist.o malloc.o

tryme.o:
	$(CC) $(CFLAGS) -c tryme.c

tryme: malloc.a
	$(CC) $(CFLAGS) tryme.o malloc.a -o tryme.out

intel-all: lib/libmalloc.so lib64/libmalloc.so

lib/libmalloc.so: lib malloc32.o
	$(CC) $(CFLAGS) -m32 -shared -o $@ malloc32.o

lib64/libmalloc.so: lib64 malloc64.o
	$(CC) $(CFLAGS) -shared -o $@ malloc64.o

lib:
	mkdir lib

lib64:
	mkdir lib64

malloc32.o: malloc.c
	$(CC) $(CFLAGS) -m32 -c -o malloc32.o malloc.c

malloc64.o: malloc.c
	$(CC) $(CFLAGS) -m64 -c -o malloc64.o malloc.c

clean:
	rm util.o freelist.o malloc.o malloc.a tryme.o tryme.out
	rm -f malloc.o *~ TAGS

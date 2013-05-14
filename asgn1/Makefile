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

lib/libmalloc.so: lib util32.o freelist32.o malloc32.o
	$(CC) $(CFLAGS) -m32 -shared -o $@ util32.o freelist32.o malloc32.o

lib64/libmalloc.so: lib64 util64.o freelist64.o malloc64.o
	$(CC) $(CFLAGS) -shared -o $@ util64.o freelist64.o malloc64.o

lib:
	mkdir lib

lib64:
	mkdir lib64

util32.o:
	$(CC) $(CFLAGS) -m32 -c -o util32.o util.c

util64.o:
	$(CC) $(CFLAGS) -m64 -c -o util64.o util.c

freelist32.o:
	$(CC) $(CFLAGS) -m32 -c -o freelist32.o freelist.c

freelist64.o:
	$(CC) $(CFLAGS) -m64 -c -o freelist64.o freelist.c

malloc32.o: malloc.c
	$(CC) $(CFLAGS) -m32 -c -o malloc32.o malloc.c

malloc64.o: malloc.c
	$(CC) $(CFLAGS) -m64 -c -o malloc64.o malloc.c

clean:
	rm util.o freelist.o malloc.o malloc.a tryme.o tryme.out
	rm -f malloc.o *~ TAGS
	rm util32.o util64.o freelist32.o freelist64.o malloc32.o malloc64.o lib/libmalloc.so lib64/libmalloc.so
	rm DETAILS DETAILS.bak
	rm -r tryAsgn1.*

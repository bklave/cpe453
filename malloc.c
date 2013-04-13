#include <stdlib.h>

void *calloc(size_t nmemb,size_t size) {
   puts("Called Girum's version of calloc()");
   return 0;
}

void *malloc(size_t size) {
   puts("called Girum's version of malloc()");
   return 0;
}

void free(void *ptr) {
   puts("called Girum's version of free()");
}

void *realloc(void *ptr, size_t size) {
   puts("called Girum's version of realloc()");
   return 0;
}

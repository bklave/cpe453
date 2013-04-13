/*
 * tryme.c
 *
 *  Created on: Apr 12, 2013
 *      Author: girum
 */

// Don't include <stdlib.h>!
#include <string.h>
#include <stdio.h>
#include "malloc.h"

int main(int argc, char *argv[]) {
	char *s;

	// Should call malloc() implicitly.
//	s = malloc(0);
//	s = malloc(sizeof(char) * 500);
//	s = malloc(sizeof(char) * 1000);
//	s = malloc(sizeof(char) * 1500);
//	s = malloc(sizeof(char) * 2000);

//	s = malloc(0);
	s = malloc(sizeof(char) * 50);
	s = malloc(sizeof(char) * 100);
	s = malloc(sizeof(char) * 150);
	s = malloc(sizeof(char) * 200);

	return 0;
}

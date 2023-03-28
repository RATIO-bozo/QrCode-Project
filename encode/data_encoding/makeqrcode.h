#ifndef MAKEQRCODE_H
#define MAKEQRCODE_H

#include "encode_string.h"

char *init_matrix(int version, size_t *size_p);

void finder(char* mat, size_t size_p);

void __finder(char* , size_t i, size_t j, size_t size_p);

void alignement(int version, char* mat, size_t size_p);

void _align(int i, int j, char* mat, size_t size_p);

void testpat(int i, int j, char* mat, size_t size_p);

void reserved(int version, char* matrix, size_t size);

void fillmatrix(char *matrix, size_t size, int *code, size_t enc);

void reserved_area(char *m, int version, int mask, size_t size);


#endif

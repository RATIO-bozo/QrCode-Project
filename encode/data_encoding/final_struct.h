#ifndef FINAL_STRUCT
#define FINAL_STRUCT

#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "polynomial.h"

int* final_struct(struct encdata* encode);
void free_message(struct poly** message,size_t t);
void encode_byte_mode_i(int* data,int c);
size_t final_byte(size_t v);

#endif

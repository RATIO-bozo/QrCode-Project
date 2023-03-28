#ifndef QRMASK
#define QRMASK

//#define _GNU_SOURCE

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "makeqrcode.h"

struct mask
{
    char *mask0;
    char *mask1;
    char *mask2;
    char *mask3;
    char *mask4;
    char *mask5;
    char *mask6;
    char *mask7;
};

struct mask *init_mask(char *qr, size_t size);

void apply_mask(struct mask *mask, char *m, size_t size, int version);
int mask_final(char **m, size_t nb_pixel, struct mask *mask, int version);

void free_mask(struct mask *mask);

#endif

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encode_string.h"

struct poly
{
    // Number of elements
    size_t size;

    // Maximal number of elements.
    size_t capacity;

    // polynomial
    struct element *data;
};

struct element
{
    // coefficient of x
    unsigned int cste;

    // power of the coefficient
    unsigned int c;

    // power of x
    unsigned int x;

    // the element is equal to (cste) ^ (c) * x ^ (x).
};

// Initialize the polynomial.
// The first element is 2 ^ 0 * x ^ 0.
struct poly *init_polynomial();

void alpha_notation(struct poly *poly);

// Reduce the polynomial
struct poly *reduction(struct poly *poly);
void __reduction(struct poly *poly, struct element *elt);

struct poly *_build_message_polynomial(char *start, size_t size);

void add_element(struct poly *poly, struct element *elt);

struct poly *distributive(struct poly *a, struct poly *b);

void integer_notation(struct poly *poly);

// multiply (a) by (b).
// the constantes must be equal or equal to 1.
void mult(struct element *a, struct element *b, struct element *res);

void print_polynomial(struct poly *poly);

// Multiply the polynomial by x ^ (power).
void polynomial_power(struct poly *poly, unsigned int power);

void free_polynomial(struct poly *poly);

struct poly *build_generator_polynomial(size_t error_codewords);

// Conversion from binary to decimal
unsigned int bin_to_dec(char *data);

// Main function for build the error correction
struct poly **build_message_polynomial(struct encdata *encode);

int *division(struct poly *mp, struct poly *gp, size_t error_codewords);

void mult_polynomial(struct poly *poly, struct element *elt);

void addition_polynomial(struct poly *a, struct poly *b);

void clone2(struct poly *src, struct poly *copy);

void __discard_leadterm(struct poly *poly, struct element *elt);

struct poly *discard_leadterm(struct poly *poly);

#endif


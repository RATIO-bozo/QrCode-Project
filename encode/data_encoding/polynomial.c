#include "polynomial.h"

const int _log[256] = {
    1,   2,   4,   8,   16,  32,  64,  128, 29,  58,  116, 232, 205, 135, 19,
    38,  76,  152, 45,  90,  180, 117, 234, 201, 143, 3,   6,   12,  24,  48,
    96,  192, 157, 39,  78,  156, 37,  74,  148, 53,  106, 212, 181, 119, 238,
    193, 159, 35,  70,  140, 5,   10,  20,  40,  80,  160, 93,  186, 105, 210,
    185, 111, 222, 161, 95,  190, 97,  194, 153, 47,  94,  188, 101, 202, 137,
    15,  30,  60,  120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225,
    223, 163, 91,  182, 113, 226, 217, 175, 67,  134, 17,  34,  68,  136, 13,
    26,  52,  104, 208, 189, 103, 206, 129, 31,  62,  124, 248, 237, 199, 147,
    59,  118, 236, 197, 151, 51,  102, 204, 133, 23,  46,  92,  184, 109, 218,
    169, 79,  158, 33,  66,  132, 21,  42,  84,  168, 77,  154, 41,  82,  164,
    85,  170, 73,  146, 57,  114, 228, 213, 183, 115, 230, 209, 191, 99,  198,
    145, 63,  126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75,
    150, 49,  98,  196, 149, 55,  110, 220, 165, 87,  174, 65,  130, 25,  50,
    100, 200, 141, 7,   14,  28,  56,  112, 224, 221, 167, 83,  166, 81,  162,
    89,  178, 121, 242, 249, 239, 195, 155, 43,  86,  172, 69,  138, 9,   18,
    36,  72,  144, 61,  122, 244, 245, 247, 243, 251, 235, 203, 139, 11,  22,
    44,  88,  176, 125, 250, 233, 207, 131, 27,  54,  108, 216, 173, 71,  142,
    1
};

const int _antilog[256] = {
    0,   0,   1,   25,  2,   50,  26,  198, 3,   223, 51,  238, 27,  104, 199,
    75,  4,   100, 224, 14,  52,  141, 239, 129, 28,  193, 105, 248, 200, 8,
    76,  113, 5,   138, 101, 47,  225, 36,  15,  33,  53,  147, 142, 218, 240,
    18,  130, 69,  29,  181, 194, 125, 106, 39,  249, 185, 201, 154, 9,   120,
    77,  228, 114, 166, 6,   191, 139, 98,  102, 221, 48,  253, 226, 152, 37,
    179, 16,  145, 34,  136, 54,  208, 148, 206, 143, 150, 219, 189, 241, 210,
    19,  92,  131, 56,  70,  64,  30,  66,  182, 163, 195, 72,  126, 110, 107,
    58,  40,  84,  250, 133, 186, 61,  202, 94,  155, 159, 10,  21,  121, 43,
    78,  212, 229, 172, 115, 243, 167, 87,  7,   112, 192, 247, 140, 128, 99,
    13,  103, 74,  222, 237, 49,  197, 254, 24,  227, 165, 153, 119, 38,  184,
    180, 124, 17,  68,  146, 217, 35,  32,  137, 46,  55,  63,  209, 91,  149,
    188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97,  242, 86,  211, 171,
    20,  42,  93,  158, 132, 60,  57,  83,  71,  109, 65,  162, 31,  45,  67,
    216, 183, 123, 164, 118, 196, 23,  73,  236, 127, 12,  111, 246, 108, 161,
    59,  82,  41,  157, 85,  170, 251, 96,  134, 177, 187, 204, 62,  90,  203,
    89,  95,  176, 156, 169, 160, 81,  11,  245, 22,  235, 122, 117, 44,  215,
    79,  174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80,  88,
    175
};

struct poly *init_polynomial()
{
    struct poly *poly = malloc(sizeof(struct poly));
    if (poly == NULL)
        errx(EXIT_FAILURE, "init_polynomial");
    poly->data = NULL;
    poly->size = 0;
    poly->capacity = 0;
    return poly;
}

void alpha_notation(struct poly *poly)
{
    for (size_t i = 0; i < poly->size; i++)
    {
        poly->data[i].c = _antilog[poly->data[i].cste];
        poly->data[i].cste = 2;
    }
}

void add_element(struct poly *poly, struct element *elt)
{
    if (poly->size == poly->capacity)
    {
        if (poly->capacity == 0)
        {
            poly->capacity = 4;
            poly->data = calloc(poly->capacity, sizeof(struct element));
        }
        else
        {
            poly->capacity *= 2;
            poly->data =
                realloc(poly->data, poly->capacity * sizeof(struct element));
        }
    }
    poly->data[poly->size].cste = elt->cste;
    poly->data[poly->size].c = elt->c;
    poly->data[poly->size].x = elt->x;
    poly->size++;
}

struct poly *distributive(struct poly *a, struct poly *b)
{
    struct poly *res = init_polynomial();
    struct element elt;
    elt.cste = 2;
    for (size_t i = 0; i < a->size; i++)
        for (size_t j = 0; j < b->size; j++)
        {
            mult(&a->data[i], &b->data[j], &elt);
            add_element(res, &elt);
        }
    return res;
}

void __reduction(struct poly *poly, struct element *elt)
{
    for (size_t i = 0; i < poly->size; i++)
    {
        if (poly->data[i].x == elt->x)
        {
            poly->data[i].cste = poly->data[i].cste ^ elt->cste;
            return;
        }
    }
    add_element(poly, elt);
}

struct poly *reduction(struct poly *poly)
{
    struct poly *res = init_polynomial(poly);
    for (size_t i = 0; i < poly->size; i++)
    {
        if (poly->data[i].cste != 0)
        {
            __reduction(res, &poly->data[i]);
        }
    }
    free_polynomial(poly);
    return res;
}

void __discard_leadterm(struct poly *poly, struct element *elt)
{
    for (size_t i = 0; i < poly->size; i++)
    {
        if (poly->data[i].x == elt->x)
        {
            poly->data[i].cste = poly->data[i].cste ^ elt->cste;
            return;
        }
    }
    add_element(poly, elt);
}

struct poly *discard_leadterm(struct poly *poly)
{
    struct poly *res = init_polynomial(poly);
    for (size_t i = 1; i < poly->size; i++)
    {
        __discard_leadterm(res, &poly->data[i]);
    }
    free_polynomial(poly);
    return res;
}

void integer_notation(struct poly *poly)
{
    for (size_t i = 0; i < poly->size; i++)
    {
        poly->data[i].cste = _log[poly->data[i].c];
        poly->data[i].c = 1;
    }
}

struct poly *_build_message_polynomial(char *start, size_t size)
{
    struct poly *poly = init_polynomial();
    poly->size = size;
    poly->capacity = size;
    poly->data = malloc(size * sizeof(struct element));
    // size_t c = size;
    size_t p = 0;

    for (size_t i = 0; i < size; i++)
    {
        poly->data[i].cste = bin_to_dec(start + p);
        poly->data[i].c = 1;
        poly->data[i].x = size - 1 - i;
        p += 8;
    }
    return poly;
}

void mult(struct element *a, struct element *b, struct element *res)
{
    if (res != NULL)
    {
        res->c = (a->c + b->c) % 255;
        res->x = a->x + b->x;
        return;
    }
    a->c = (a->c + b->c) % 255;
    a->x += b->x;
}

void print_polynomial(struct poly *poly)
{
    printf("=====================================\n");
    printf("    Size : %zu\n", poly->size);
    printf("    Capacity : %zu\n", poly->capacity);
    for (size_t i = 0; i < poly->size; i++)
    {
        printf("%3i ^ %3i * x ^ %2i\n", poly->data[i].cste, poly->data[i].c,
               poly->data[i].x);
    }
    printf("=====================================\n");
}

void polynomial_power(struct poly *poly, unsigned int power)
{
    struct element tmp;
    tmp.x = power;
    tmp.cste = 2;
    tmp.c = 0;
    for (size_t i = 0; i < poly->size; i++)
    {
        mult(&poly->data[i], &tmp, NULL);
    }
}

void free_polynomial(struct poly *poly)
{
    free(poly->data);
    free(poly);
}

struct poly *build_generator_polynomial(size_t error_codewords)
{
    struct poly *res = init_polynomial();
    struct poly *tmp = init_polynomial();
    struct poly *res_tmp;
    struct element e1;
    struct element e2;
    e1.cste = 2;
    e2.cste = 2;
    e1.c = 0;
    e2.c = 0;
    e1.x = 1;
    e2.x = 0;
    add_element(res, &e1);
    add_element(res, &e2);
    add_element(tmp, &e1);
    add_element(tmp, &e2);
    for (size_t i = 1; i < error_codewords; i++)
    {
        tmp->data[1].c = (unsigned int)i;
        res_tmp = distributive(res, tmp);
        free_polynomial(res);
        res = res_tmp;
        integer_notation(res);
        res = reduction(res);
        alpha_notation(res);
    }
    free_polynomial(tmp);
    return res;
}

unsigned int bin_to_dec(char *data)
{
    unsigned int s = 0;
    unsigned int compt = 1;
    for (unsigned int i = 8; i > 0; i--)
    {
	if(data[i-1] == '1'){
		s+=compt;
	}
        compt *= 2;
        
    }
    return s;
}

struct poly **build_message_polynomial(struct encdata *encode)
{
    // Encode Message polynomial
    size_t t = encode->block1 + encode->block2;
    struct poly **message = malloc(sizeof(struct poly) * t);
    size_t y = 0;
    for (size_t i = 0; i < encode->block1; i++) // Encode Group 1
    {
        message[i] = _build_message_polynomial(encode->data + y, encode->group1);
        y += (encode->group1 * 8);
    }

    for (size_t i = 0; i < encode->block2; i++) // Encode Group 2
    {
        message[encode->block1 + i] =
            _build_message_polynomial(encode->data + y, encode->group2);
        y += encode->group2 * 8;
    }
    return message;
}

int *division(struct poly *mp, struct poly *gp, size_t error_codewords)
{
    struct element power;
    power.c = 0;
    power.x = -1;
    power.cste = 0;

    struct poly *res = init_polynomial();
    struct poly *gp2 = init_polynomial();

    clone2(mp, res);
    polynomial_power(res, error_codewords);
    polynomial_power(gp, res->data[0].x - gp->data[0].x);
    clone2(gp, gp2);

    struct element lead_term;
    lead_term.cste = 2;
    lead_term.x = 0;

    lead_term.c = _antilog[res->data[0].cste];
    mult_polynomial(gp2, &lead_term);
    integer_notation(gp2);
    addition_polynomial(res, gp2);
    res = discard_leadterm(res);

    for (size_t i = 0; i < mp->size - 1; i++)
    {
        mult_polynomial(gp, &power);
        clone2(gp, gp2);
        lead_term.c = _antilog[res->data[0].cste];
        mult_polynomial(gp2, &lead_term);
        integer_notation(gp2);
        addition_polynomial(res, gp2);
        res = discard_leadterm(res);
    }

    int *coef = malloc(error_codewords * sizeof(int));
    for (size_t i = 0; i < error_codewords; i++)
    {
        coef[i] = res->data[i].cste;
    }
    free_polynomial(gp2);
    free_polynomial(res);
    return coef;
}

void clone2(struct poly *src, struct poly *copy)
{
    copy->size = src->size;
    copy->capacity = src->capacity;
    if (copy->data == NULL)
    {
        copy->data = malloc(copy->capacity * sizeof(struct element));
    }
    for (size_t i = 0; i < copy->size; i++)
    {
        copy->data[i].cste = src->data[i].cste;
        copy->data[i].x = src->data[i].x;
        copy->data[i].c = src->data[i].c;
    }
}

void addition_polynomial(struct poly *a, struct poly *b)
{
    size_t i = 0;
    for (; i < a->size; i++)
    {
        if (i >= b->size)
            return;
        a->data[i].cste = (a->data[i].cste ^ b->data[i].cste);
    }
    for (; i < b->size; i++)
    {
        struct element elt;
        elt.cste = b->data[i].cste;
        elt.x = b->data[i].x;
        elt.c = b->data[i].c;
        add_element(a, &elt);
    }
}

void mult_polynomial(struct poly *poly, struct element *elt)
{
    for (size_t i = 0; i < poly->size; i++)
    {
        mult(&poly->data[i], elt, NULL);
    }
}


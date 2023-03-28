#include "encode_string.h"

struct encdata *data_encoding(char *input, char *cl) // main function
{
    struct encdata *qr = malloc(sizeof(struct encdata));
    if (qr == NULL)
        err(EXIT_FAILURE, "data_encoding: malloc");
    qr->size = strlen(input);
    qr->correction_level = *cl;
    // Add the length of the mode indicator
    qr->len = 4;
    // Determine the mode indicator
    qr->mi = check_input(input);

    if (qr->mi == 0)
        err(EXIT_FAILURE, "mode_indicator: Couldn't encode the input string");

    size_t count_indicator = smallest_version(qr);

    set_mode_indicator(qr);

    add_character_count_indicator(qr, count_indicator);

    encode_input(qr, input);

    add_terminator(qr);

    pad_bytes(qr);

    return qr;
}

int check_input(char *input)
{
    for (size_t i = 0; i < strlen(input); i++)
    {
        if (input[i] < 32)
            // lower than 32 is not a printable caracter
            return 0;
    }
    return 1;
}

size_t smallest_version(struct encdata *qr)
{
    size_t len = qr->size;
    char *line = NULL;
    size_t buf_len = 0;
    qr->version = 1;

    FILE *file = fopen("./encode/data_encoding/m_byte_mode", "r");
    if (file == NULL)
        errx(EXIT_FAILURE, "path doesn't exist");

    while (getline(&line, &buf_len, file) >= 0)
    {
        size_t n1, n2, n3, n4, n5, n6, n7;
        sscanf(line, "%zu %zu %zu %zu %zu %zu %zu", &n1, &n2, &n3, &n4, &n5,
               &n6, &n7);
        qr->nlen = n2;
        qr->ec = n3;
        qr->block1 = n4;
        qr->group1 = n5;
        qr->block2 = n6;
        qr->group2 = n7;
        if (n1 < len)
            qr->version += 1;
        else
            break;
    }
    free(line);
    fclose(file);
    size_t count_ind = 0;
    if (qr->version <= 9)
        count_ind = 8;
    else
        count_ind = 16;

    return count_ind;
}

void set_mode_indicator(struct encdata *qr)
{
    qr->data = malloc(qr->nlen);
    qr->data = memset(qr->data,0,qr->nlen);
    if (qr->data == NULL)
        err(EXIT_FAILURE, "mode_indicator: malloc()");
    char *ptr = qr->data;
    if (qr->mi == 1)
    {
        *ptr = '0';
        ptr++;
        *ptr = '1';
        ptr++;
        *ptr = '0';
        ptr++;
        *ptr = '0';
        ptr++;
    }
}

void add_character_count_indicator(struct encdata *qr, size_t count_indicator)
{
    size_t tmp = count_indicator;
    char *ptr = qr->data + qr->len + count_indicator - 1;
    char size = qr->size;
    while (count_indicator > 0 && size >= 0)
    {
        if (size % 2 == 0)
            *ptr = '0';
        else
            *ptr = '1';
        size = size >> 1;
        count_indicator--;
        ptr--;
    }
    while(count_indicator>0){
	*ptr='0';
	count_indicator--;
	ptr--;
    }
    qr->len += tmp;
}

void encode_input(struct encdata *qr, char *input)
{
    if (qr->mi == 1)
    {
        //qr->data = realloc(qr->data, qr->len + qr->size * 8);
        if (qr->data == NULL)
            err(EXIT_FAILURE, "encode_input: malloc()");
        for (size_t i = 0; i < qr->size; i++)
        {
            __encode_byte(qr->data + qr->len, input[i]);
            qr->len += 8;
        }
    }
}

void __encode_byte(char *data, char c)
{
    size_t i = 8;
    data += 7;
    while (i > 0 && c >= 0)
    {
        if (c % 2 == 0)
            *data = '0';
        else
            *data = '1';
        c = c >> 1;
        i--;
        data--;
    }
    while(i>0){
	*data='0';
	i--;
	data--;
    }
}

void add_terminator(struct encdata *qr)
{
    size_t i = 4;
    char *ptr = qr->data + qr->len;
    while (qr->len < qr->nlen && i > 0)
    {
        *ptr = '0';
        ptr++;
        qr->len++;
        i--;
    }
}

void pad_bytes(struct encdata *qr)
{
    char *ptr = qr->data + qr->len;
    while ((qr->len) % 8 != 0)
    {
        *ptr = '0';
        ptr++;
        qr->len++;
    }
    int state = 0;
    while (qr->len < qr->nlen)
    {
        // printf("%zu\n", qr->len);
        qr->len += 8;
        if (state == 0)
        {
            *ptr = '1';
            ptr++;
            *ptr = '1';
            ptr++;
            *ptr = '1';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '1';
            ptr++;
            *ptr = '1';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '0';
            ptr++;
        }
        else
        {
            *ptr = '0';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '1';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '0';
            ptr++;
            *ptr = '1';
            ptr++;
        }
        state = (state + 1) % 2;
    }
}

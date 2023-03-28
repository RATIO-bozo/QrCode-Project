#include "final_struct.h"

void free_message(struct poly **message, size_t t)
{
    for (size_t i = 0; i < t; i++)
    {
        free(message[i]->data);
        free(message[i]);
    }
    free(message);
}

int *final_struct(struct encdata* encode)
{
    // Encode Message polynomial
    struct poly **message = build_message_polynomial(encode);

    // Encode Generator polynomial
    struct poly *generator = build_generator_polynomial(encode->ec);

    // Encode Error correction
    size_t t = encode->block1 + encode->block2; // size of array

    int *ec[t];
    for (size_t i = 0; i < t; i++)
    { 
        ec[i] = division(message[i], generator, encode->ec);
	printf("Error code value : %d\n\n",*ec[i]);
    }

    size_t f = final_byte(encode->version);
    // Build final message
    size_t all = (encode->block1 * encode->group1
                  + encode->block2 * encode->group2 + t * encode->ec)
            * 8
        + f;

    int *final = malloc(sizeof(int) * all);
    int inc = 0;
    for (size_t x = 0; x < encode->group1; x++)
    {
        for (size_t y = 0; y < t; y++)
        {
            encode_byte_mode_i(final + inc, message[y]->data[x].cste);
            // printf("\n");
            inc += 8;
        }
    }

    if (encode->group1 < encode->group2)
    {
        for (size_t x = encode->group1; x < encode->group2; x++)
        {
            for (size_t y = encode->block1; y < t; y++)
            {
                encode_byte_mode_i(final + inc, message[y]->data[x].cste);
                // printf("\n");
                inc += 8;
            }
        }
    }

    for (size_t x = 0; x < encode->ec; x++)
    {
        for (size_t y = 0; y < t; y++)
        {
            encode_byte_mode_i(final + inc, ec[y][x]);
            // printf("\n");
            inc += 8;
        }
        // printf("========\n");
    }

    for (size_t x = 0; x < f; x++)
        final[inc + x] = 0;

    free_message(message, t);
    free_polynomial(generator);
    for (size_t i = 0; i < t; i++)
        free(ec[i]);

    return final;
}

void encode_byte_mode_i(int *data, int c)
{
    size_t i = 8;
    data += 7;
    while (i > 0 && c > 0)
    {
        if (c % 2 == 0)
            *data = 0;
        else
            *data = 1;
        c = c >> 1;
        i--;
        data--;
    }
    while (i > 0)
    {
        *data = 0;
        i--;
        data--;
    }
}

size_t final_byte(size_t v)
{
    char *line = NULL;
    size_t buf_len = 0;
    FILE *file = fopen("./encode/data_encoding/final_byte", "r");
    size_t byte = 0;

    while (v != 0)
    {
        ssize_t line_len = getline(&line, &buf_len, file);
	if(line_len == -1){
		errx(1,"Can't read this line");
	}
        v--;
    }

    sscanf(line, "%zu", &byte);
    free(line);
    fclose(file);

    return byte;
}

#ifndef ENCODE_STRING_H
#define ENCODE_STRING_H

#define  _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct encdata
{
    // Version of the QR code.
    int version;

    // Mode indicator.
    // 1 -> byte mode
    char mi;

    // Length of the input string.
    size_t size;

    // Error correction level.
    char correction_level;

    // Length of the encoded data.
    size_t len;

    // Length necessary for the encoded data.
    size_t nlen;

    // Encoded data (binary).
    char *data;

    // Error correction Codewords Per Block
    size_t ec;

    // Number of blocks in group 1
    size_t block1;

    // Number of Data Codewords in Each of Group 1's Blocks
    size_t group1;

    // Number of Blocks in Group 2
    size_t block2;

    // Number of Data Codewords in Each of Group 2's Blocks
    size_t group2;
};

// Return (1) if the input can be encoded in byte mode.
// Otherwise return (0).
int check_input(char *input);

// Main function for encoding.
struct encdata *data_encoding(char *input, char *cl);

// Add the mode indicator in the data field in qr.
void set_mode_indicator(struct encdata *qr);

// Return (1) if the input can be encoded in byte mode (ISO-8859-1).
// Otherwise return (0).
// count the number of the char
int check_input(char *input);

// Encode the input string in the appropriate mode indicator.
// Update the data field in qr.
void encode_input(struct encdata *qr, char *input);

// Add the terminator into the data field.
void add_terminator(struct encdata *qr);

// Encode the input text in byte mode (ISO-8859-1).
// Then update the the data field in qr.
void __encode_byte(char *data, char c);

// Add the character count indicator in the data field.
void add_character_count_indicator(struct encdata *qr, size_t count_indicator);

// Determine the smallest version (size of the QR code) for the data
// Considering the smallest version is 21x21 pixels and each version is
// 4 pixels larger.
size_t smallest_version(struct encdata *qr);

// Pad bytes to fill the total capacity of the QR code.
void pad_bytes(struct encdata *qr);


#endif

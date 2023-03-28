#ifndef VARIABLES_H
#define VARIABLES_H

int **matrix; //2d matrix that represents the QrCode.
size_t N; //Matrix size (of a side).
size_t V; //QrCode version.

size_t encodingMode;

size_t ECC; // Error Correction level
size_t mask; // Mask of the QrCode


size_t dataLength; //Length of the data in bits
char *data; // String representing our data in an bit-array form.


size_t charIndicatorLength; // Length of the charIndicator in bits
size_t charIndicator; // Length of the message in letters
char* message; // The decoded message of the QrCode

#endif

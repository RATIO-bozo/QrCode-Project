#ifndef PATTERNS_H
#define PATTERNS_H

void fillFindingPatterns(int **matrix, size_t N);
void fillFormatInformation(int **matrix, size_t N);
void fillTimingPatterns(int **matrix, size_t N);
void fillAlignmentPatterns(int **matrix, size_t N, size_t V);
void fillPatterns(int **matrix, size_t N, size_t V);

int isOutOfBounds(int x, int y, size_t N);
int isNextToVerticalPattern(int x);
int isData(int **matrix, int x, int y);

#endif

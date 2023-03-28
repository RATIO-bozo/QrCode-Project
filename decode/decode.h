#ifndef DECODE_H
#define DECODE_H

void initFromFile(char *path);
void initFromChar(char* matrix, size_t len);
void freeMatrix();
void freeDecodeAll();
int getCharIndicatorLength();
void initPosition(size_t version);
int getYDir(int x, int y);
int isInAligmentPattern(size_t x, size_t y);
void printMatrix();
int dataSquares(int iStart, int jStart, int iEnd, int jEnd);
void getFormat();
int mustSkip();
int isModuleData(int x, int y);
void invert(int x, int y);
void unMask(int mask);
void extractData(int xDecrement, int startRow);
void getDataLength();
char alphaToChar(char alphaIndex);
char* getFinalMessage();
char* decode();
void decode_main(char* path,char* msg);

#endif

#ifndef QrCodeAnalyser_H
#define QrCodeAnalyser_H

SDL_Surface* load_image(const char* path);

int** getPixelMatrix(SDL_Surface* image);

#endif

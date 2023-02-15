#ifndef CUT_H
#define	CUT_H

SDL_Surface* load_image(const char* path);

int get_index(SDL_Surface* surface,int i);
int is_corner (SDL_Surface* surface,int i);
int is_corner_bis (SDL_Surface* surface,int i, int size);
SDL_Surface* crop_qrcode(SDL_Surface *image, int x, int y, int w, int h);

void getPixelMatrix(SDL_Surface* image, int pixelMatrix[image->h][image->w]);

#endif

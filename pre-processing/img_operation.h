#ifndef IMG_OPERATION_H
#define IMG_OPERATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 getpixel(SDL_Surface* surface, int x,int y);
size_t find_resolution(SDL_Surface* img);
void find_first_black(SDL_Surface* img,int* start_x,int* start_y);

#endif

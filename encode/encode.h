#ifndef ENCODE_H
#define ENCODE_H

#include <stdlib.h>
#include <string.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./data_encoding/encode_string.h"
#include "./data_encoding/polynomial.h"
#include "./data_encoding/makeqrcode.h"
#include "./data_encoding/final_struct.h"
#include "./data_encoding/qrmask.h"

void setPixel(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, size_t x,
              size_t y);

int encode(char* argv);

#endif

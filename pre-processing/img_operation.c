#include "img_operation.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	    case 1:
		return *p;
		break;

	    case 2:
		return *(Uint16 *)p;
		break;

	    case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		    return p[0] << 16 | p[1] << 8 | p[2];
		return p[0] | p[1] << 8 | p[2] << 16;
		break;

		case 4:
		    return *(Uint32 *)p;
		    break;

		default:
		    return 0;       /* shouldn't happen, but avoids warnings */
	}
}

size_t find_resolution(SDL_Surface* img){
	size_t resolution = 1;
	SDL_LockSurface(img);
	SDL_Color rgb;
	int start=0;
	int end = 0;
	size_t len = 0;
	for(int x=0; x<img->w;x++){
		for(int y=0; y<img->h;y++){
			Uint32 data = getpixel(img,x,y);
			SDL_GetRGB(data,img->format,&rgb.r,&rgb.g,&rgb.b);
			if(rgb.r+rgb.g+rgb.b == 0){
				if(start == 0){
					start = 1;
					len++;
				}
				else{
					len++;
				}			
			}
			else{
				if(start == 1){
					end = 1;
					break;
				}
			}			
		}
		if(end==1){
			break;
		}
	}
	resolution = len/7;
	SDL_UnlockSurface(img);
	return resolution;
		
}

void find_first_black(SDL_Surface* img,int* start_x,int* start_y){
	SDL_Color rgb;
	for(int x=0; x<img->w;x++){
                for(int y=0; y<img->h;y++){
			Uint32 data = getpixel(img,x,y);
                        SDL_GetRGB(data,img->format,&rgb.r,&rgb.g,&rgb.b);
                        if(rgb.r+rgb.g+rgb.b == 0){
				*start_x=x;
				*start_y=y;
				return;
			}

		}
	}
}

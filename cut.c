#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    SDL_Surface* n_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(surface);
    return n_surface;
}

//get the index of the first black pixel if no pixel is found return -1
int get_index(SDL_Surface* surface,int i) 
{
    Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;
    Uint8 r,g,b;
    int size_large =  surface->w;
	int size_haut = surface->h;

    int index = i;
    while(index<size_haut*size_large)
    {
        SDL_GetRGB(pixels[index],format,&r,&g,&b);
        if (r<10 && g<10 && b<10)
        {
            return index; 
        }
        pixels[index] = SDL_MapRGB(format,1,1,250);
        index +=1;
    }
    return -1;
}

//verify if the i index is a corner of a QrCode

int is_corner (SDL_Surface* surface,int i)
{
    Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;
    Uint8 r,g,b;
    int size_large =  surface->w;
	int size_haut = surface->h; 

    int temp = i%size_haut;
    int size= 0;
    SDL_GetRGB(pixels[i],format,&r,&g,&b);
    while (temp<size_large && r<10 && g<10 && b<10)
    {    
    	pixels[i] = SDL_MapRGB(format,250,1,1);
        i++;
        temp++;
        size++;
        
        SDL_GetRGB(pixels[i],format,&r,&g,&b);
    }
    size--;
    int tempL = i-size;
    int tempR = i;
    Uint8 r1,g1,b1;
    SDL_GetRGB(pixels[tempL],format,&r,&g,&b);
    SDL_GetRGB(pixels[tempR],format,&r1,&g1,&b1);
    while (size>0 && tempL/size_large<size_haut && tempR/size_large<size_haut && r<10 && g<10 && b<10 && r1<10 && g1<10 && b1<10)
    {
            pixels[tempL] = SDL_MapRGB(format,1,1,250);
            pixels[tempR] = SDL_MapRGB(format,1,1,250);
            tempL+=size_large;
            tempR+=size_large;
            size--;
            SDL_GetRGB(pixels[tempL],format,&r,&g,&b);
            SDL_GetRGB(pixels[tempR],format,&r1,&g1,&b1);
    }
    if (size == 0)
    {
        IMG_SavePNG(surface,"test.png");
        return 1;
    }
    return 0;
}


int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    
    SDL_Surface* surface = load_image(argv[1]);
    int i= get_index(surface,0);
    if (i == -1 )
         errx(EXIT_FAILURE, "No QrCode");
    is_corner (surface,i);
    IMG_SavePNG(surface,"pretraitement.PNG");
    return 0;
}

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
        //pixels[index] = SDL_MapRGB(format,1,1,250);
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
    int size = 0;
    SDL_GetRGB(pixels[i],format,&r,&g,&b);
    while (temp<size_large && r<10 && g<10 && b<10)
    {    
        i++;
        temp++;
        size++;
        SDL_GetRGB(pixels[i],format,&r,&g,&b);
    }

    int res = size;
    if (res<=30)
        errx(EXIT_FAILURE, "No QrCode");
    
    // left side
    int tempL = i-size;
    SDL_GetRGB(pixels[tempL],format,&r,&g,&b);
    while (size>0 && tempL<size_haut*size_large && r<10 && g<10 && b<10)
    {
            
            //pixels[tempL] = SDL_MapRGB(format,250,1,1);
            tempL+=size_large;
            size--;
            SDL_GetRGB(pixels[tempL],format,&r,&g,&b);
    }

    
    //left side is working

    //right side
    int size2 = res;
    i--;
    SDL_GetRGB(pixels[i],format,&r,&g,&b);
    while (size2>0 && i<size_haut*size_large && r<10 && g<10 && b<10)
    {
        //pixels[i] = SDL_MapRGB(format,250,1,1);
        i+=size_large;
        size2--;
        SDL_GetRGB(pixels[i],format,&r,&g,&b);
    }

    //right side os working

    //bottom
    while (tempL<i && r<10 && g<10 && b<10)
    {
        tempL++;
        SDL_GetRGB(pixels[tempL],format,&r,&g,&b);
    }
    //bottom is working

    if (size == 0 && size2==0 && tempL>=i)
    {
        return res;
    }
    return -1;
}



int is_corner_bis (SDL_Surface* surface,int i, int size)
{
    Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;
    Uint8 r,g,b;
    int size_large =  surface->w;
	int size_haut = surface->h; 

    int temp = i;
    int tempsize = size;
    SDL_GetRGB(pixels[temp],format,&r,&g,&b);
    while (tempsize>0 && temp<size_large*size_haut && r<10 && g<10 && b<10)
    {   
        //pixels[temp] = SDL_MapRGB(format,1,250,1);
        temp++;
        tempsize--;
        SDL_GetRGB(pixels[temp],format,&r,&g,&b);
    }
    if (tempsize>0)
        return -temp;
    
    // left side
    tempsize = size;
    SDL_GetRGB(pixels[i],format,&r,&g,&b);
    while (tempsize>0 && i<size_haut*size_large && r<10 && g<10 && b<10)
    {
            //pixels[i] = SDL_MapRGB(format,1,250,1);
            i+=size_large;
            tempsize--;
            SDL_GetRGB(pixels[i],format,&r,&g,&b);
    }
    if (tempsize>0)
        return -1;

    //left side is working

    //right side
    tempsize = size-1;
    temp--;
    SDL_GetRGB(pixels[temp],format,&r,&g,&b);
    while (tempsize>0 && temp<size_haut*size_large && r<10 && g<10 && b<10)
    {
        //pixels[temp] = SDL_MapRGB(format,1,250,1);
        temp+=size_large;
        tempsize--;
        SDL_GetRGB(pixels[temp],format,&r,&g,&b);
    }
    if (tempsize>0)
        return -1;

    //right side os working

    //bottom
    while (i<temp && r<10 && g<10 && b<10)
    {
        i++;
        SDL_GetRGB(pixels[i],format,&r,&g,&b);
    }
    //bottom is working
    
    if (i>=temp)
    {
        return 1;
    }
    return -1;
}

SDL_Surface* crop_qrcode(SDL_Surface *image, int x, int y, int w, int h) 
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    SDL_Surface* rogner = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    
    SDL_BlitSurface(image,&rect,rogner,NULL);
    SDL_FreeSurface(image);
    return rogner;
}



void getPixelMatrix(SDL_Surface* image, int pixelMatrix[image->h][image->w]) {
   int n = image->h;
   int m = image->w;
  // Loop through each pixel in the image and determine if it's black or white
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < m; x++) {
      Uint32 pixel = *((Uint32*)image->pixels + y * image->pitch / 4 + x);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      int average = (r + g + b) / 3;
      pixelMatrix[y][x] = (average >= 128) ? 0 : 1;
    }
  }
  return;
}
/*
void mat_to_qrMat(int n,int mat[n][n],int largeur_carre,int matQr[largeur_carre][largeur_carre] )
{
	int i = 0;
	while (mat[0][i+1]==1)
	{
		i++;
	}
	int j = 0
	while (mat[j+1][i]==1)
	{
		j++;
	}
	
}*/

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    
    SDL_Surface* surface = load_image(argv[1]);


    Uint32* pixels = surface->pixels;
	SDL_PixelFormat* format = surface->format;
    Uint8 r,g,b;
    int size_large =  surface->w;
	int size_haut = surface->h; 

    //try to find upperleft corner
    
    int index= get_index(surface,0);
    if (index == -1 )
         errx(EXIT_FAILURE, "No QrCode");

    int size = is_corner (surface,index);
    int x =0;
    while (size == -1)
    {
            index= get_index(surface,index);
            if (index!=-1)
                size = is_corner (surface,index);
            else 
                errx(EXIT_FAILURE, "No QrCode");
    }
    x = index;

    //corner found 

    int boo = -1;
    while (boo<=-1)
    {
        
        index++;
        if (index<size_haut*size_large)
        {
            //pixels[i] = SDL_MapRGB(format,1,250,1);
            SDL_GetRGB(pixels[index],format,&r,&g,&b);
            if (r<10)
            {
                boo = is_corner_bis (surface,index,size);
                if (boo<0)
                    index+=size;
            }
        }
        else 
            errx(EXIT_FAILURE, "No QrCode");

    }
    int largeur = index + size - x;

    int y = x/size_large;
    x = x%size_large;
    surface=  crop_qrcode(surface,x-1,y, largeur, largeur);

    IMG_SavePNG(surface,"pretraitement.PNG");
    
    
    int n = surface->h;
    int m = surface->w;
    int mat[n][m];
    getPixelMatrix(surface,mat);
    
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", mat[k][j]);
        }
        printf("\n");
    }
    SDL_FreeSurface(surface);
    return 0;
}

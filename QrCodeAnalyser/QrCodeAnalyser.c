#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "QrCodeAnalyser.h"

#define QRCODE_SIZE 29
#define VERSION_INFORMATION_SIZE 6


int main(int argc, char** argv)
	{
		if (argc != 2)
			errx(EXIT_FAILURE, "Usage: image-file");

		int versionInformation[VERSION_INFORMATION_SIZE];

		int version = 0;

		// remplis la matrice en pixels
		SDL_Surface* surface = load_image(argv[1]);
		int** mat=  getPixelMatrix(surface);

		int  qrcode_pixels = surface -> w * surface -> h;

		int pixel_size = qrcode_pixels / (QRCODE_SIZE * QRCODE_SIZE);

		//initialise la matrice pour ensuite la remplire de carre plutot que de pixels
		int squareQRCode[QRCODE_SIZE / pixel_size][QRCODE_SIZE/pixel_size];

		//transformation de la matrice de pixels en une matrice de carres
		int k, i, j, ii, jj;
		for(i = 0; i < QRCODE_SIZE; i += pixel_size){
			for(j = 0; j < QRCODE_SIZE; j += pixel_size) {
				int sum = 0;
				for (ii = i; ii < i + pixel_size; ii++) {
					for (jj = j; jj < j + pixel_size; jj++) {
						sum += squareQRCode[ii][jj];
					}
				}
				squareQRCode[i / pixel_size][j / pixel_size] = (sum == 0) ? 1 : 0;
			}
		}

		printf("bjr");

		for(int count = 0; count < QRCODE_SIZE / pixel_size; count ++)
		{
			for(int count2 = 0; count2 < QRCODE_SIZE/pixel_size; count2++){
				printf("%d", squareQRCode[count][count2]);
			}
		}

		/*

		//detecte le cham version information
		i, j, k = 0;
		for(i = 0; i < 6; i++)
		{
			for(j = QRCODE_SIZE - 11; j < QRCODE_SIZE -11 + 3; j++)
			{
				versionInformation[k++]= squareQRCode[i][j];
			}
		}


		//recuperation de la vesion


		for(i = 0; i < 6; i++)
		{
			version = (version << 1) | versionInformation[i];
		}

		printf("version info : ");
		for(i = 0; i < 6; i++)
		{
			printf("%d", versionInformation[i]);
		}
		printf("\n");
		printf("version: %d\n", version);
*/
		return 0;
	}


SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    SDL_Surface* n_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(surface);
    return n_surface;
}



int** getPixelMatrix(SDL_Surface* image) {
  int** pixelMatrix;
  int width = image->w;
  int height = image->h;

  // Allocate memory for the pixel matrix
  pixelMatrix = (int**) malloc(height * sizeof(int*));
  for (int i = 0; i < height; i++) {
    pixelMatrix[i] = (int*) malloc(width * sizeof(int));
  }

  // Loop through each pixel in the image and determine if it's black or white
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Uint32 pixel = *((Uint32*)image->pixels + y * image->pitch / 4 + x);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image->format, &r, &g, &b);
      int average = (r + g + b) / 3;
      pixelMatrix[y][x] = (average >= 128) ? 0 : 1;
    }
  }

  return pixelMatrix;
}

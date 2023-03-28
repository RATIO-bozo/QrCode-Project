#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img_operation.h"
#include "load.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void print_matrix(char *matrix, size_t *size_p)
{
    printf("=================================================\n");
    size_t size = *size_p;
    for (size_t i = 0; i < *size_p; i++)
    {
        for (size_t j = 0; j < *size_p; j++)
        {
            if (matrix[i * size + j] == '0')
                printf(" 0");
            else if (matrix[i * size + j] == '1')
            {
                printf(" 1");
            }
            else if (matrix[i *size + j]=='3')
            {
                printf(" 3");
            }
            else
            {
                printf(" 2" );
            }

        }
        printf("\n");
    }
    printf("=================================================\n");
}




int main(){
	SDL_Surface* img = load_image("out.bmp");
	size_t res = find_resolution(img);
	int start_x,start_y;
	SDL_LockSurface(img);
	find_first_black(img,&start_x,&start_y);
	SDL_UnlockSurface(img);
	size_t matrix_size = (img->w-(2*start_x))/res;
	printf("Resolution of a square: %ld\nCoordinates of first black pixel: %d - %d \nSize: %ld \n ",res,start_x,start_y,matrix_size);
	char* matrix = malloc(sizeof(char) * matrix_size * matrix_size);
	if(matrix == NULL){
		return 1;
	}
	SDL_Color rgb;
	for(int i=start_x; i< ((img->w)-start_x);i+=res){
		for(int j=start_y; j< ((img->h)-start_y);j+=res){
			Uint32 data = getpixel(img,i,j);
                        SDL_GetRGB(data,img->format,&rgb.r,&rgb.g,&rgb.b);
                        if(rgb.r+rgb.g+rgb.b == 0){
				matrix[((j-start_x)/res)*matrix_size+((i-start_y)/res)]='1';
			}
			else{
				matrix[((j-start_x)/res)*matrix_size+((i-start_y)/res)]='0';
			}
		}
	}
	print_matrix(matrix,&matrix_size);
	free(matrix);
}

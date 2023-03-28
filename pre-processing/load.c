#include "load.h"
#include "err.h"

SDL_Surface* load_image(char path[]){
	if(!strlen(path)){
		printf("need the path of the image");
		return NULL;
	}

	SDL_Surface* image = NULL;
	image = IMG_Load(path);
	if(!image){
		errx(1,"Load_image : bad image path");
	}
	return image;
}

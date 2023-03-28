#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>



SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    SDL_Surface* n_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(surface);
    return n_surface;
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

int detect_qrcode_version(int module_matrix[29][29], int size) {
    int version = 0;
    int max_module_count = 0;
    
    // Parcours des motifs de référence des versions de QrCode
    for (int v = 1; v <= 40; v++) {
        int module_count = 0;

        // Détection des modules de référence
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                if (size-11+j >= 0 && size-11+j < 29 && module_matrix[i][size-11+j] == 1) {
                    module_count++;
                }
                if (size-11+j >= 0 && size-11+j < 29 && module_matrix[size-11+j][i] == 1) {
                    module_count++;
                }
            }
        }
        for (int i = size-6; i < size; i++) {
            for (int j = 0; j < 3; j++) {
                if (module_matrix[i][size-11+j] == 1) {
                    module_count++;
                }
                if (module_matrix[size-11+j][i] == 1) {
                    module_count++;
                }
            }
        }

        // Mise à jour de la version maximale détectée
        if (module_count > max_module_count) {
            max_module_count = module_count;
            version = v;
        }
    }

    return version+2;
}

int main() {
    int size = 29; // taille de la matrice de QrCode
    
    SDL_Surface* surface = load_image("image/elbaz.bmp");
    
    int mat[size][size];
    getPixelMatrix(surface,mat);
    
    int qrcode_version = detect_qrcode_version(mat, size);
    
    printf("***************************************************************\n\n");
    for (int k = 0; k < size; k++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", mat[k][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("***************************************************************\n");
    
    printf("Version du QrCode : %d\n", qrcode_version);
    printf("dimension du QrCode : %dx%d\n",size,size);

    // Libération de la mémoire allouée
    
    SDL_FreeSurface(surface);
    return 0;
}

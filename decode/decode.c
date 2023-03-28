#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../pre-processing/img_operation.h"
#include "../pre-processing/load.h"

#include "decode.h"
#include "patterns.h"
#include "colors.h"

char alphaTable[9] = {' ', '$', '%', '*', '+', '-', '.', '/', ':' };

int **matrix = NULL; //2d matrix that represents the QrCode.
size_t N = 0; //Matrix size (of a side).
size_t V = 0; //QrCode version.

size_t encodingMode = 0;

size_t ECC = 0; // Error Correction level
size_t mask = 0; // Mask of the QrCode


size_t dataLength = 0; //Length of the data in bits
char *data = NULL; // String representing our data in an bit-array form.


size_t charIndicatorLength = 0; // Length of the charIndicator in bits
size_t charIndicator=0; // Length of the message in letter

void initWithChar(char* charMatrix, size_t len)
{
    N = (size_t) sqrt((double) len);

    if (N * N != len)
        errx(1, "The Qrcode isn't squared (the number of modules is %lu, which isn't a square.\n", len);
    if (N < 21)
        errx(1, "The QrCode size must be N*N with N >= 21 and N <= 177\n");
   
    /* Now that we stocked all the elements and that we have the size of the matrix N
    ** We can create our 2D matrix.
    */
    
    matrix = malloc(sizeof(int *) * N);
    for (size_t y = 0; y < N; y++)
    {
        matrix[y] = malloc(sizeof(int) * N);
        for (size_t x = 0; x < N; x++)
            matrix[y][x] = charMatrix[y * N + x] - '0';
    }

    printf("Loading done !\n");

}

void initWithFile(char *path)
{
    printf("Loading the matrix from file \"%s\"...\n", path);
    
    FILE *file = fopen(path, "r");
    size_t maxLength = 177 * 177;

    int list[maxLength]; //Initialize with the max length possible.
    size_t total = 0; 
    int c;

    if (file == NULL) 
        errx(1, "Couldn't not open the file");

    while ((c = fgetc(file)) != EOF) {
        if (c == '1' || c == '0')
        {
            if (total == maxLength)
                errx(1, "The file have to much modules, should be < %lu\n", maxLength);
            list[total++] = c - '0';
        }
    }
    fclose(file);
    
    N = (size_t) sqrt((double) total);

    if (N * N != total)
        errx(1, "The Qrcode isn't squared (the number of modules is %lu, which isn't a square.\n", total);
    if (N < 21)
        errx(1, "The QrCode size must be N*N with N >= 21 and N <= 177\n");

    /* Now that we stocked all the elements and that we have the size of the matrix N
    ** We can create our 2D matrix.
    */
    
    matrix = malloc(sizeof(int *) * N);
    for (size_t y = 0; y < N; y++)
    {
        matrix[y] = malloc(sizeof(int) * N);
        for (size_t x = 0; x < N; x++)
            matrix[y][x] = list[y * N + x];
    }

    printf("Loading done !\n");
}


void freeMatrix()
{
    for (size_t i = 0; i < N; i++)
        free(matrix[i]);
    free(matrix);
}

void freeDecodeAll(){
    freeMatrix();
    free(data);
}


void _pause()
{
    printf("Press Enter to resume the program. \n");
    int tmp = getchar();
    tmp--;
}


int getCharIndicatorLength()
{
    if (1 <= V && V <= 9)
    {
        if (encodingMode == 1)
            return 10;
        else if (encodingMode == 2)
            return 9;
        else
            return 8;
    }
    else if (V <= 26)
    {
        if (encodingMode == 1)
            return 12;
        else if (encodingMode == 2)
            return 11;
        else if (encodingMode == 4)
            return 16;
        else
            return 10;
    }
    else if (V <= 40)
    {
        if (encodingMode == 1)
            return 14;
        else if (encodingMode == 2)
            return 13;
        else if (encodingMode == 4)
            return 16;
        else
            return 12;
    }
    
    // This line will never be reached, just to avoid compilation problems.
    return -1;
}


void printDebug()
{
    for (size_t y = 0; y < N; y++)
    {
        for (size_t x = 0; x < N; x++)
        {
            printf("%i ", matrix[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}


void printMatrix()
{
    /*
    printf("Patterns colours: ");
    yellow();
    printf("Finder patterns - ");
    blue();
    printf("Format information - ");
    purple();
    printf("Timing pattern - ");
    cyan();
    printf("Aligment pattern\n");

    reset(); 
    printf("Data colours: "); 
    black();
    printf("Black data - ");
    white();
    printf("White data - ");
    red();
    printf("Module skipped -");
    green();
    printf("Module read\n");
    */
    char *square = "\u25A0 ";

    for (size_t y = 0; y < N; y++)
    {
        for (size_t x = 0; x < N; x++)
        {
            int m = matrix[y][x];
            if (m == 0)
               black();
            else if (m == 1)
                white();
            else if (m == 2)
                green();
            else if (m == 3)
                red();
            else if (m == 4)
                yellow();
            else if (m == 5)
                blue();
            else if (m == 6)
                purple();
            else if (m == 7)
                cyan();

            printf("%s", square);
        }
        printf("\n");
    }
    printf("\n");
    white();
}


int dataSquares(int iStart, int jStart, int iEnd, int jEnd)
{
    int res = 0;
    int iIncrement = iEnd > iStart ? 1 : -1;
    int jIncrement = jEnd > jStart ? 1 : -1;

    for (int i = iStart; i != (iEnd + iIncrement); i += iIncrement)
    {
        for (int j = jStart; j != (jEnd + jIncrement); j += jIncrement)
        {
            res = (res << 1) + matrix[i][j];
        }
    }
    return res;
}


void getFormat()
{
    size_t firstPart = dataSquares(N-1, 8, N-7, 8) << 8;
    size_t secondPart = dataSquares(8, N-8, 8, N-1);

    size_t formatString = firstPart + secondPart;

    ECC = dataSquares(N-1, 8, N-2, 8);

    if (formatString == 30660 || formatString == 21522 || formatString == 13663 || formatString == 5769)
        mask = 0;
    else if (formatString == 29427 || formatString == 20773 || formatString == 12392 || formatString == 5054)
        mask = 1;
    else if (formatString == 32170 || formatString == 24188 || formatString == 16177 || formatString == 7399)
       mask = 2;
    else if (formatString == 30877 || formatString == 23371 || formatString == 14854 || formatString == 6608)
       mask = 3;
    else if (formatString == 26159 || formatString == 17913 || formatString == 9396  || formatString == 1890)
       mask = 4;
    else if (formatString == 25368 || formatString == 16590 || formatString == 8579  || formatString == 597)
       mask = 5;
    else if (formatString == 27713 || formatString == 20375 || formatString == 11994 || formatString == 3340)
       mask = 6;
    else if (formatString == 26998 || formatString == 19104 || formatString == 11245 || formatString == 2107)
       mask = 7;
}

void invert(int x, int y)
{
    matrix[y][x] = (matrix[y][x] == 0 ? 1 : 0);
}

void unMask(int mask)
{
    for (size_t x = 0; x < N; x++)
    {
        for (size_t y = 0; y < N; y++)
        {
            if (isData(matrix, x, y))
            {
                if (mask == 0 && ((y + x) % 2 == 0))
                    invert(x, y);
                else if (mask == 1 && (y % 2 == 0))
                    invert(x, y);
                else if (mask == 2 && (x % 3 == 0))
                    invert(x, y);
                else if (mask == 3 && ((y + x) % 3 == 0))
                    invert(x, y);
                else if (mask == 4 && ((y / 2 + x / 3) % 2 == 0))
                    invert(x, y);
                else if (mask == 5 && ((((y * x) % 2) + ((y * x) % 3)) == 0))
                    invert(x, y);
                else if (mask == 6 && (((((y * x) % 2) + ((y * x) % 3)) % 2) == 0))
                    invert(x, y);
                else if (mask == 7 && (((((y + x) % 2) + ((y * x) % 3)) % 2) == 0))
                    invert(x, y);
            }
        }
    }
}


void extractData(int xDecrement, int startRow)
{
    int yDir = -1;
    size_t num_bits = 0;

    int xInit = N-1;
    int x = xInit - xDecrement, y = startRow;

    data = malloc(sizeof(char) * dataLength + 1);

    while (num_bits < dataLength)
    {
        /*
        printf("Currently at (%i, %i) \n", y, x);
        printf("xDecrement = %i, xInit = %i\n", xDecrement, xInit);
        printf("yDir = %i\n", yDir);
        */

        if (isData(matrix,x,y))
        {
            // printf("Module can be considered \n");
            data[num_bits++] = '0' + matrix[y][x];
            matrix[y][x] = 2;
        }
        else
        {
            // printf("Module must be skipped \n");
            matrix[y][x] = 3;
        }

        //printMatrix();
        printf("Data = ");
        
        for (size_t i = 0; i < num_bits; i++)
            printf("%c", data[i]);
        printf("\n");

        xDecrement = (xDecrement+1)%2;

        y += (xDecrement % 2) ? 0 : yDir;
        x = xInit - xDecrement;

        if (isOutOfBounds(x, y, N))
        {
            yDir = yDir == -1 ? 1 : -1;
            y += yDir;

            xInit -= isNextToVerticalPattern(x) ? 3 : 2;
            xDecrement = 0;
            x = xInit - xDecrement;
        }
	sleep(0.7);
    }
}


void getDataLength()
{
    if (encodingMode == 1)
        dataLength = (charIndicator / 3) * 10 + ((charIndicator % 3 == 2) ? 7 : 4);
    else if (encodingMode == 2)
        dataLength = (charIndicator / 2) * 11 + 6 * (charIndicator % 2);
    else if (encodingMode == 4)
        dataLength = charIndicator * 8;
    else if (encodingMode == 8)
        errx(1, "Sorry, the Kanji encoding mode has yet to be implemented.");
}


char alphaToChar(char alphaIndex)
{
    // This functions convert a alphanumeric table index
    // and return the char equivalent.
    if (alphaIndex == 0)
        return 0;

    if (alphaIndex < 0 && alphaIndex > 44)
        errx(1, "The QrCode had encoded an invalid alphanumeric index : %c", alphaIndex);
    if (0 <= alphaIndex && alphaIndex <= 9)
        return '0' + alphaIndex;
    else if (alphaIndex <= 35)
        return 'A' + (alphaIndex - 10);
    else
        return alphaTable[alphaIndex - 36];
}


char* getFinalMessage()
{
    // This function convert the Bits Representation "data"
    // into the raw string using the equivalent encode mode used.

    char* message = malloc(sizeof(char) * (charIndicator + 1));
    size_t index = 0;

    if (encodingMode == 1)
    {
        int c = 0;
        size_t i;
        for (i = 0; i < dataLength; i++)
        {
            if (i % 3 == 0)
            {
                printf("%i", c);
                message[index++] = '0' + c;
            }
            c = (c << 1) + (data[i] - '0');
        }
        if (i % 3 != 0)
        {
            printf("%i", c);
            message[index++] = '0' + c;
        }
        message[index] = 0;
    }
    else if (encodingMode == 2)
    {
        char c = 0;
        size_t i;
        for (i = 0; i < dataLength; i++)
        {
            if (i % 2 == 0)
            {
                int x = c / 45;
                int y = c % 45;
                char xChar = alphaToChar(x);
                char yChar = alphaToChar(y);
                message[index++] = xChar;
                message[index++] = yChar;
            }
        }
        if (i % 2 == 0)
        {
            int x = c / 45;
            int y = c % 45;
            char xChar = alphaToChar(x);
            char yChar = alphaToChar(y);
            message[index++] = xChar;
            message[index++] = yChar;
        }
        else
        {
            char xChar = alphaToChar(c);
            message[index++] = xChar;
        }
        message[index] = 0;
    }
    else if (encodingMode == 4)
    {
        char c = 0;
        for (size_t i = 0; i < dataLength; i++)
        {
            if (i % 8 == 0 && i != 0)
            {
                message[index++] = c;
                c = 0;
            }
            c = (c << 1) + (data[i] - '0');
        }
        message[index++] = c;
        message[index++] = 0;
    }
    return message;
}

void getMessageLength(int maxRow){
	char *line = NULL;
	size_t buf_len = 0;
	size_t nb_block = 0;
	size_t i = V;
	FILE *file = fopen("./encode/data_encoding/m_byte_mode", "r");
    	if (file == NULL)
        	errx(EXIT_FAILURE, "path doesn't exist");

   	 while (getline(&line, &buf_len, file) >= 0)
    	{
        	size_t n1, n2, n3, n4, n5, n6, n7;
        	sscanf(line, "%zu %zu %zu %zu %zu %zu %zu", &n1, &n2, &n3, &n4, &n5,&n6, &n7);
		nb_block = n4+n6;
		if(i==0){
			break;
		}
		else{
			i--;
		}
	}
	if(V >= 4){
		int start=dataSquares(N-3, N-1, N-4, N-2);
		int end = dataSquares(N-(nb_block*4)-1,N-1,N-(nb_block*4)-2,N-2);
		//printf("Start : %d ,End = %d, Istart = %ld",start,end,nb_block*4);
		charIndicator = start*16 + end;
	}
	else{
		charIndicator=dataSquares(N-3, N-1, maxRow, N-2);
	}
}
char* decode()
{
    /*
    ** Knowing the equation N = (((V-1)*4)+21)
    ** We can compute the version V
    */

    V = (N-21) / 4 + 1;
    printf("QrCode is version V, with V = %lu\n", V);

    if (1 > V || V > 40)
        errx(1, "The QrCode version must be >= 1 and <= 40");

    getFormat();
    printf("QrCode has a ECC of %lu, and a mask of %lu.\n", ECC, mask);

    printf("The coloured version of this QrCode would be: \n");
    //printMatrix();
    //_pause();

    printf("Now we detect the patterns: \n");

    fillPatterns(matrix, N, V);

    printf("Then, we apply the mask (mask %li) \n", mask);
    unMask(mask);
    printf("UnMasking Done !\n");

    //printMatrix();
    //_pause();

    encodingMode = dataSquares(N-1, N-1, N-2, N-2);
    printf("QrCode has an encoding mode of %lu ", encodingMode);
    if (encodingMode == 1)
        printf("(Numeric mode\n)");
    else if (encodingMode == 2)
        printf("(Alphanumeric mode)\n");
    else if (encodingMode == 4)
        printf("(Byte Mode)\n");
    else if (encodingMode == 8)
        printf("(Kanji Mode)\n");
    else if (encodingMode == 7)
        printf("(ECI Mode)\n");
    else
        errx(1, "Unvalid encoding mode: %lu", encodingMode);

    
    charIndicatorLength = getCharIndicatorLength();
    
    int maxRow = N - (2 + charIndicatorLength/2);

    getMessageLength(maxRow);

    if (charIndicatorLength%2 == 1)
        charIndicator = (charIndicatorLength << 1) + matrix[maxRow - 1][N-1];

    printf("The character Indicator is coded on %li bits. \n", charIndicatorLength);
    printf("QrCode has a message of %li characters.\n", charIndicator);
    
    getDataLength();
    printf("The data length in bits is %lu.\n",dataLength);

    int startRow = N - (3 + charIndicatorLength/2);
    int xDecrement = (charIndicatorLength % 2);
    extractData(xDecrement, startRow);

    char* message = getFinalMessage();

    freeMatrix();
    free(data);

    return message;
}

void decode_main(char* path,char* msg)
{

    if(path == NULL)
        return;

    SDL_Surface* img = load_image(path);
    size_t res = find_resolution(img);
    int start_x,start_y;
    SDL_LockSurface(img);
    find_first_black(img,&start_x,&start_y);
    SDL_UnlockSurface(img);
    size_t matrix_size = (img->w-(2*start_x))/res;
    char* matrix = malloc(sizeof(char) * matrix_size * matrix_size);
    

    SDL_Color rgb;

    for(int i=start_x; i< ((img->w)-start_x);i+=res){
            for(int j=start_y; j< ((img->h)-start_y);j+=res){
                    Uint32 data = getpixel(img,i,j);
                    SDL_GetRGB(data,img->format,&rgb.r,&rgb.g,&rgb.b);
                    if(rgb.r+rgb.g+rgb.b == 0){
                            matrix[((j-start_x)/res)*matrix_size + ((i-start_y)/res)]='1';
                    }
                    else{
                            matrix[((j-start_x)/res)*matrix_size + ((i-start_y)/res)]='0';
                    }
            }
    }
    
    //initWithFile("data/v3.txt");
    // or
    initWithChar(matrix, matrix_size * matrix_size);

    
    char * message2 = decode();
    msg = strcpy(msg, message2);

    free(message2);

    free(matrix);



}



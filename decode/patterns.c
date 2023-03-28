#include <stdlib.h>

#include "patterns.h"


int findingPattern = 4;
int formatInformation = 5;
int TimingPattern = 6;
int aligmentPattern = 7;


void fillFindingPatterns(int** matrix, size_t N)
{
    for (size_t x = 0; x < 8; x++)
    {
        for (size_t y = 0; y < 8; y++)
        {
            matrix[y][x] = findingPattern;
            matrix[y][N-1-x] = findingPattern;
            matrix[N-1-y][x] = findingPattern;
        }
    }
}


void fillFormatInformation(int** matrix, size_t N)
{
    for (size_t i = 0; i < 7; i++)
        matrix[N-1-i][8] = formatInformation;
    for (size_t i = 0; i < 8; i++)
        matrix[8][N-1-i] = formatInformation;

    for (size_t i = 0; i < 6; i++)
        matrix[8][i] = formatInformation;

    matrix[8][7] = formatInformation;
    matrix[8][8] = formatInformation;
    matrix[7][8] = formatInformation;
    
    for (size_t i = 0; i < 6; i++)
        matrix[i][8] = formatInformation;
}


void fillTimingPatterns(int** matrix, size_t N)
{
    for (size_t i = 8; i < (N - 8); i++)
    {
        matrix[6][i] = TimingPattern;
        matrix[i][6] = TimingPattern;
    }

    matrix[N-8][8] = TimingPattern;
}


void fillAlignmentPatterns(int** matrix, size_t N, size_t V)
{
    if (V == 1)
        return;

    // ----------------------------------------------------------------
    // Computing the positions of centers of aligments patterns

    size_t num_positions;
    size_t *positions;

    num_positions = V / 7 + 2;

    positions = malloc(sizeof(size_t) * num_positions);
    
    size_t firstPos = 6;
    size_t lastPos = N - 1 - firstPos;
    size_t secondLastPos = firstPos + lastPos * (num_positions - 2);
    secondLastPos += (num_positions - 1) / 2;
    secondLastPos /= (num_positions - 1);
    secondLastPos -= (secondLastPos % 2);

    size_t posStep = lastPos - secondLastPos;
    size_t secondPos = lastPos - (num_positions - 2) * posStep;

    positions[0] = firstPos;
    for (size_t pos = secondPos, i = 1; pos <= lastPos; pos += posStep)
        positions[i++] = pos;

    // ----------------------------------------------------------------

    size_t xCenter, yCenter; // Coords of center of aligment pattern

    int isTopLeft, isTopRight, isBottomLeft;

    for (size_t xIndex = 0; xIndex < num_positions; xIndex++)
    {
        xCenter = positions[xIndex];

        for (size_t yIndex = 0; yIndex < num_positions  ; yIndex++)
        {
            yCenter = positions[yIndex];

            // First, we have to test if the aligment pattern exist
            // (i.e not in the finder pattern)

            isTopLeft = (yCenter == firstPos && xCenter == firstPos);
            isTopRight = (yCenter == firstPos && xCenter == lastPos);
            isBottomLeft = (yCenter == lastPos && xCenter == firstPos);

            if (!isTopLeft && !isTopRight && !isBottomLeft)
            {
                for (size_t x = xCenter - 2; x <= xCenter + 2; x++)
                {
                    for(size_t y = yCenter - 2; y <= yCenter + 2; y++)
                        matrix[y][x] = aligmentPattern;
                }
            }
        }
    }

    free(positions);
}


void fillPatterns(int** matrix, size_t N, size_t V)
{
    fillFindingPatterns(matrix, N);
    fillFormatInformation(matrix, N);
    fillTimingPatterns(matrix, N);
    fillAlignmentPatterns(matrix, N, V);
}


int isOutOfBounds(int x, int y, size_t N)
{
    return x < 0 || x >= (int) N || y < 0 || y >= (int) N;
}


int isNextToVerticalPattern(int x)
{
    return x == 6;
}


int isData(int** matrix, int x, int y)
{
    return (matrix[y][x] == 1 || matrix[y][x] == 0);
}


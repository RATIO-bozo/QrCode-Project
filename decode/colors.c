#include <stdio.h>
#include "colors.h"

void black()
{
    printf("\033[1;30m");
}


void white()
{
    printf("\033[1;37m");
}

void green()
{
    printf("\033[1;32m");
}

void red()
{
    printf("\033[1;31m");
}

void yellow()
{
    printf("\033[1;33m");
}

void blue()
{
    printf("\033[1;34m");
}

void purple()
{
    printf("\033[1;35m");
}

void cyan()
{
    printf("\033[1;36m");
}

void reset()
{
    printf("\033[0m");
}

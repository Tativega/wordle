#include <stdio.h>

void normalPrint()
{
    printf("\033[0m");
}

void printGreen()
{
    printf("\033[30;42m");
}

void printYellow()
{
    printf("\033[30;43m");
}

void printWhite()
{
    printf("\033[30;47m");
}
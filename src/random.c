#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    int nbNodes = atoi(argv[1]);
    int nbEdges = atoi(argv[2]);
    int nbTerminals = atoi(argv[3]);
    int matrix[nbNodes][nbNodes];
    int* terminals = createTerminals(nbTerminals);
    int i;
    for(i = 0; i < nbTerminals; i++)
    {
        printf("%d\n", terminals[i]);
    }
    return 0;
}

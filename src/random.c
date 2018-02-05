#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    int nbNodes = atoi(argv[1]);
    int nbEdges = atoi(argv[2]);
    int matrix[nbNodes][nbNodes];
    initMatrix(nbNodes, matrix);
    displayMatrix(nbNodes, matrix);
    printf("\n\n");
    createRandomMatrix(nbNodes, nbEdges, matrix);
    displayMatrix(nbNodes, matrix);
    return 0;
}

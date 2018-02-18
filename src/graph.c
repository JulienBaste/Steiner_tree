#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "utils.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    int nbTerminals;
    int nbBags;
    int nbNodes;
    int nbEdges;
    int bagSize;
    int i;
    char buffer[BUFSIZE];

    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        perror("fopen");
    }

    getNbNodes(file, &nbNodes);
    getNbEdges(file, &nbEdges);
    int matrix[nbNodes][nbNodes];
    initMatrix(nbNodes, matrix);
    fillMatrix(file, nbNodes, nbEdges, matrix);/* Bug a corriger sur nbEdges*/

    for(i = 0; i < 34; i++)
    {
        fgetc(file);
    }
    fscanf(file, "%d", &nbTerminals);

    while(strcmp(buffer, "SECTION Tree Decomposition\n") != 0)
    {
        fgets(buffer, BUFSIZE, file);
    }

    if(fgetc(file) == 'c')
    {
        fgets(buffer, BUFSIZE, file);
    }

    for(i = 0; i < 4; i++)
    {
        fgetc(file);
    }

    fscanf(file, "%d %d", &nbBags, &bagSize);

    int* bags[nbBags];

    for(i = 0; i < 5; i++)
    {
        fgetc(file);
    }

    fillBags(file, bags, nbBags, bagSize);

    /*int* edgesTD[nbBags++];
    initEdgesAtZero(edgesTD, 0, nbBags++);
    fillEdgesTD(file, edgesTD);*/

    /*for(i = 0; i < nbBags++; i++)
    {
        printArray(edgesTD[i]);
    }*/

    fclose(file);

    return 0;
}

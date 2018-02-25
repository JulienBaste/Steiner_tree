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
    int maxEdges = 1;
    int nbTerminals;
    int nbBags;
    int nbNodes;
    int nbEdges;
    int bagSize;
    int i;
    int* terminals;
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
    fillMatrix(file, nbNodes, nbEdges, matrix);

    for(i = 0; i < 34; i++)
    {
        fgetc(file);
    }

    fscanf(file, "%d", &nbTerminals);
    terminals = malloc(sizeof(int) * nbTerminals);
    fillTerminals(file, terminals, nbTerminals);
    terminals = triFusion(nbTerminals, terminals);

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

    for(i = 0; i < 4; i++)
    {
        fgetc(file);
    }

    fillBags(file, bags, nbBags, bagSize);

    for(i = 1; i < nbBags; i++)
    {
        bags[i] = triFusion(bagSize, bags[i]);
    }

    int* edgesTD[nbBags + 1];
    initEdgesAtZero(edgesTD, 0, nbBags + 1);
    maxEdges = fillEdgesTD(file, edgesTD, maxEdges, nbBags + 1);

    for(i = 1; i < nbBags + 1; i++)
    {
        edgesTD[i] = triFusion(maxEdges, edgesTD[i]);
    }

    int* parcouru = malloc(sizeof(int) * (nbBags+1));
    initArray(nbBags, parcouru);

    int* res = findFirstTerminal(bags, edgesTD, nbBags, maxEdges, nbTerminals, terminals, parcouru, 1);

    /*niceTD root;
    root.bag = malloc(sizeof(int) * bagSize);
    initArray(bagSize, root.bag);
    initArray(parcouru);
    root.bag[0] = bags[res[0]][res[1]];

    buildNiceTD(root, bags, edgesTD, nbBags, bagSize, maxEdges, res[0], parcouru);*/

    fclose(file);

    return 0;
}

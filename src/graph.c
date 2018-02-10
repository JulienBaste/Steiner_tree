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
    fillMatrix(file, nbNodes, nbEdges, matrix);

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

    fscanf(file, "%d", &nbBags);

    printf("%d %d %d %d\n", nbNodes, nbEdges, nbTerminals, nbBags);
    fclose(file);

    return 0;
}

/*//affichage de matrice
//remplir la matrice avec des 0
//existence d'un arrete
//ajout et suppression d'arrete*/

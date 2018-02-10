#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "utils.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    int nbTerminals;
    int nbBags;
    int treeWidth;
    int nbNodes;
    int nbEdges;
    int u;
    int v;
    int w;
    int i = 0;

    char buffer[BUFSIZE];

    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        perror("fopen");
    }
    fseek(file, 20, SEEK_SET);
    fscanf(file, "%d", &nbNodes);
    int matrix[nbNodes][nbNodes];
    initMatrix(nbNodes, matrix);
    fseek(file, 7, SEEK_CUR);
    fscanf(file, "%d", &nbEdges);
    for(i; i < nbEdges; i++)
    {
        fseek(file, 3, SEEK_CUR);
        fscanf(file, "%d %d %d", &u, &v, &w);
        createEdges(nbNodes, matrix, u, v, w);
    }
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
    fscanf(file, "%d %d %d\n", &nbBags, &treeWidth, &nbNodes);
    fclose(file);

    return(0);
}

/*//affichage de matrice
//remplir la matrice avec des 0
//existence d'un arrete
//ajout et suppression d'arrete*/

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "utils.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
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
    fseek(file, 7, SEEK_CUR);
    fscanf(file, "%d", &nbEdges);
    for(i; i < nbEdges; i++)
    {
        fseek(file, 3, SEEK_CUR);
        fscanf(file, "%d %d %d", &u, &v, &w);
        matrix[u][v] = w;
        matrix[v][u] = w;
    }
    displayMatrix(nbNodes, matrix);
    printf("\n\n\n\n", );

    fgets(buffer, BUFSIZE, file);
    while(buffer[0] != 'S')
    {
        fgets(buffer, BUFSIZE, file);
    }
    fgets(buffer, BUFSIZE, file);
    if(buffer[0] == 'c')
    {
        fgets(buffer, BUFSIZE, file);
    }
    fseek(file, 5, SEEK_CUR);
    fscanf(file, "%d %d", &nbBags, &treeWidth);
    printf("%d %d\n", nbBags, treeWidth);
    fclose(file);

    return(0);
}

/*//affichage de matrice
//remplir la matrice avec des 0
//existence d'un arrete
//ajout et suppression d'arrete*/

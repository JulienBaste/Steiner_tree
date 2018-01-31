#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int nbNodes;
    int nbEdges;
    int u;
    int v;
    int w;
    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        perror("fopen");
    }
    fseek(file, 20, SEEK_SET);
    fscanf(file, "%d", &nbNodes);
    int matrice[nbNodes][nbNodes];
    fseek(file, 7, SEEK_CUR);
    fscanf(file, "%d", &nbEdges);
    for(int i = 0; i < nbEdges; i++)
    {
        fseek(file, 3, SEEK_CUR);
        fscanf(file, "%d %d %d", &u, &v, &w);
        matrice[u][v] = w;
        matrice[v][u] = w;
    }
    fclose(file);
}

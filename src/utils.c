#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

void initMatrix(int size, int matrix[][size])
{
    int i;
    int j;
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void displayMatrix(int size, int matrix[][size])
{
    int i;
    int j;
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void createEdges(int size, int matrix[][size], int u, int v, int w)
{
    matrix[u][v] = w;
}

int ifEdgesExist(int size, int matrix[][size], int u, int v)
{
    if(matrix[u][v] > 0)
    {
        return(matrix[u][v]);
    }
    return(0);
}

void deleteEdges(int size, int matrix[][size], int u, int v)
{
    matrix[u][v] = 0;
}

void createRandomMatrix(int nbNodes, int nbEdges, int matrix[][nbNodes])
{
    int i = 0;
    int u;
    int v;
    int w;

    while(i < nbEdges)
    {
        u = (rand() % (nbNodes)) + 1;
        v = (rand() % (nbNodes)) + 1;
        w = (rand() % (100)) + 1;
        if(u == v) continue;
        else
        {
            matrix[u][v] = w;
            i++;
        }
    }
}

int* createRandomTerminals(int nbTerminals, int nbNodes)
{
    int res[nbTerminals];
    int i;
    for(i = 0; i < nbTerminals; i++)
    {
        res[i] = (rand() % (nbNodes)) + 1;
    }
    return res;
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

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
    matrix[v][u] = w;
}

int ifEdgesExist(int size, int matrix[][size], int u, int v)
{
    if(matrix[u][v] > 0)
    {
        return(matrix[u][v]);
    }
    else if(matrix[v][u] > 0)
    {
        return(matrix[v][u]);
    }
    return(0);
}

void deleteEdges(int size, int matrix[][size], int u, int v)
{
    matrix[u][v] = 0;
    matrix[v][u] = 0;
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
            createEdges(nbNodes, matrix, u, v, w);
            i++;
        }
    }
}

int* createTerminals(int nbTerminals)
{
    int* res = malloc(sizeof(int) * nbTerminals);
    int i;
    for(i = 1; i <= nbTerminals; i++)
    {
        res[i-1] = i;
    }
    return res;
}



/*void combination(char n[], int k, int startPosition, char result[])
{
    printf("%s\n", n);
    int i;
    int tmp = strlen(n) - k;
    printf("%s\n", result);
    if(k == 0)
    {
        printf("%s\n", result);
    }
    for(i = startPosition; i <= tmp; i++)
    {
        printf("%d %d\n", i, tmp);
        printf("%c\n", n[i]);
        result[startPosition] = n[i];
        printf("resultTMP : %c\n", result[tmp]);
        printf("result : %s\n", result);
        combination(n, k-1, i+1, result);
    }
}

int* steiner(int size, int matrix[][size], int nbTerminals, int terminals)
{
    int i;
    for(i = 0; i < nbTerminals; i++)
    {

    }
}*/

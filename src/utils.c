#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

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

void initArray(int size, int* array)
{
    int i;

    for(i = 0; i < size; i++)
    {
        array[i] = 0;
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

void createRandomMatrix(int nbNodes, int nbEdges, int* matrix)
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

void getNbNodes(FILE* file, int* nbNodes)
{
    fseek(file, 20, SEEK_SET);
    fscanf(file, "%d", nbNodes);
}

void getNbEdges(FILE* file, int* nbEdges)
{
    fseek(file, 7, SEEK_CUR);
    fscanf(file, "%d", nbEdges);
}

void fillMatrix(FILE* file, int nbNodes, int nbEdges, int matrix[][nbNodes])
{
    int u;
    int v;
    int w;
    int i;

    for(i = 0; i < nbEdges; i++)
    {
        fseek(file, 3, SEEK_CUR);
        fscanf(file, "%d %d %d", &u, &v, &w);
        createEdges(nbNodes, matrix, u, v, w);
    }
}

void fillTerminals(FILE* file, int* terminals, int nbTerminals)
{
    int i;

    for(i = 0; i < nbTerminals; i++)
    {
        fseek(file, 3, SEEK_CUR);
        fscanf(file, "%d", &terminals[i]);
    }
}

void fillBags(FILE* file, int** bags, int nbBags, int bagSize)
{
    int i;
    int tmp;
    int idBag;
    int size;

    for(i = 0; i < nbBags; i++)
    {
        size = 0;
        int* bag = malloc(sizeof(int) * bagSize);
        initArray(bagSize, bag);
        fgetc(file);
        fscanf(file, "%d", &idBag);
        while(fgetc(file) == ' ')
        {
            fscanf(file, "%d", &tmp);
            bag[size] = tmp;
            size++;
        }
        bags[idBag] = bag;
    }
}

int fillEdgesTD(FILE* file, int** edgesTD, int maxEdges, int nbEdge)
{
    int i;
    int u;
    int v;
    int index;

    while(fscanf(file, "%d %d", &u, &v) > 0)
    {
        index = nextZero(maxEdges, edgesTD[u]);
        if(index == -1)
        {
            maxEdges = maxEdges * 2;
            for(i = 0; i < nbEdge; i++)
            {
                int* newEdge = malloc(sizeof(int) * maxEdges);
                initArray(maxEdges, newEdge);
                copyArray(maxEdges/2, edgesTD[i], newEdge);
                if(i == u)
                {
                    newEdge[(maxEdges/2)] = v;
                }
                edgesTD[i] = newEdge;
            }
        }
        else
        {
            edgesTD[u][index] = v;
        }
        index = nextZero(maxEdges, edgesTD[v]);
        edgesTD[v][index] = u;
    }

    return maxEdges;
}

int nextZero(int size, int* array)
{
    int i;

    for(i = 0; i < size; i++)
    {
        if(array[i] == 0) return i;
    }

    return -1;
}

void initEdgesAtZero(int** edges, int first, int last)
{
    int i;

    for(i = first; i <= last; i++)
    {
        edges[i] = malloc(sizeof(int) * 1);
        initArray(1, edges[i]);
    }
}

void copyArray(int size, int* t1, int* t2)
{
    int i;

    for(i = 0; i < size; i++)
    {
        t2[i] = t1[i];
    }
}

void printArray(int size, int* t)
{
    int i;

    for(i = 0; i < size; i++)
    {
        printf("%d ", t[i]);
    }
    printf("\n");
}

void buildNiceTD(niceTD tree, int** bags, int** edges, int nbBags, int bagSize, int maxEdges, int next)
{
    int* bag = bags[next];
}

int* findFirstTerminal(int** bags, int** edges, int bagSize, int maxEdges, int nbTerminals, int* terminals, int* parcouru, int next)
{
    int i;
    int* bag = bags[next];
    int* res = malloc(sizeof(int) * 2);
    int* tmp = malloc(sizeof(int) * 2);

    for(i = 0; i < bagSize; i++)
    {
        if(bag[i] == 0) break;
        if(dichotomie(bag[i], nbTerminals, 0, terminals) != -1)
        {
            res = malloc(sizeof(int) * 2);
            res[0] = next;
            res[1] = i;
            return res;
        }
    }

    int* fils = edges[next];
    parcouru[next] = 1;
    res[0] = maxEdges;
    res[1] = bagSize;

    for(i = 0; i < maxEdges; i++)
    {
        if(fils[i] != 0 && parcouru[fils[i]] == 0)
        {
            tmp = findFirstTerminal(bags, edges, bagSize, maxEdges, nbTerminals, terminals, parcouru, fils[i]);
            if(tmp[0] < res[0])
            {
                res = tmp;
            }
            else if(tmp[0] == res[0] && tmp[1] < res[1])
            {
                res = tmp;
            }
        }
    }

    return res;
}

int* put(int e, int size, int* tab)
{
    int i;
    int* res = malloc(sizeof(int) * size+1);

    res[0] = e;
    for(i = 1; i <= size; i++)
    {
        res[i] = tab[i-1];
    }

    return res;
}

int* triFusion(int size, int* tab)
{
    if(size <= 1) return tab;
    else
    {
        int newSize = size/2;
        int* tmp = malloc(sizeof(int) * (newSize));
        copyArray(newSize, tab, tmp);
        return fusion(newSize, newSize+size%2, triFusion(newSize, tmp), triFusion(newSize+size%2, &tab[newSize]));
    }
}

int* fusion(int sizeT1, int sizeT2, int* t1, int* t2)
{
    if(sizeT1 == 0) return t2;
    if(sizeT2 == 0) return t1;
    if((t1[0] <= t2[0] && t1[0] != 0) || t2[0] == 0)
    {
        return put(t1[0], sizeT1+sizeT2, fusion(sizeT1-1, sizeT2, &t1[1], t2));
    }
    else
    {
        return put(t2[0], sizeT1+sizeT2, fusion(sizeT1, sizeT2-1, t1, &t2[1]));
    }
}

int dichotomie(int e, int size, int res, int* tab)
{
    if(size == 1)
    {
        if(tab[0] == e) return res;
        return -1;
    }
    int i = size/2;
    if(tab[i] == e) return res+i;
    if(tab[i] > e || tab[i] == 0)
    {
        int* tmp = malloc(sizeof(int) * i);
        copyArray(i, tab, tmp);
        return dichotomie(e, i, res, tmp);
    }
    else
    {
        int* tmp = malloc(sizeof(int) * i+size%2);
        copyArray(i+size%2, &tab[i], tmp);
        return dichotomie(e, i+size%2, res+i, tmp);
    }
}

<<<<<<< HEAD
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

void buildNiceTD(niceTD* tree, int** bags, int** edges, int nbBags, int bagSize, int maxEdges, int next, int* parcouru)
{
    niceTD* tmp = tree;
    int i;
    int index;
    int newNext;
    int* toGo;
    int* bag = bags[next];
    int* fils = edges[next];
    int* tmp2;
    if(nextZero(bagSize, bag) != 0)
    {
        int** newNode = cmpBags(bagSize, tree->bag, bag);

        for(i = 0; i < bagSize; i++)
        {
            if(newNode[0][i] == 0) break;
            niceTD* son = constructor(2, bagSize);
            copyArray(bagSize, tmp->bag, son->bag);
            index = dichotomie(newNode[0][i], bagSize, 0, son->bag);
            son->bag[index] = 0;
            tmp2 = triFusion(bagSize, son->bag);
            free(son->bag);
            son->bag = tmp2;
            tmp->type = 1;
            tmp->left = son;
            tmp = son;
        }

        for(i = 0; i < bagSize; i++)
        {
            if(newNode[1][i] == 0) break;
            niceTD* son = constructor(1, bagSize);
            copyArray(bagSize, tmp->bag, son->bag);
            index = nextZero(bagSize, son->bag);
            son->bag[index] = newNode[1][i];
            tmp2 = triFusion(bagSize, son->bag);
            free(son->bag);
            son->bag = tmp2;
            tmp->type = 2;
            tmp->left = son;
            tmp = son;
        }

        free(newNode[0]);
        free(newNode[1]);
        free(newNode);
    }

    parcouru[next] = 1;
    toGo = toVisit(maxEdges, nbBags+1, fils, parcouru);

    while(toGo[0] > 2)
    {
        niceTD* leftSon = constructor(2, bagSize);
        niceTD* rightSon = constructor(2, bagSize);
        copyArray(bagSize, tmp->bag, leftSon->bag);
        copyArray(bagSize, tmp->bag, rightSon->bag);
        tmp->left = leftSon;
        tmp->right = rightSon;
        tmp->type = 3;

        index = nextZero(maxEdges+1, toGo);
        newNext = toGo[index-1];
        toGo[index-1] = 0;
        toGo[0] = toGo[0] - 1;

        tmp = rightSon;
        buildNiceTD(leftSon, bags, edges, nbBags, bagSize, maxEdges, newNext, parcouru);
    }

    if(toGo[0] == 2)
    {
        buildNiceTD(tmp, bags, edges, nbBags, bagSize, maxEdges, toGo[1], parcouru);
    }

    if(toGo[0] == 1)
    {
        index = nextZero(bagSize, tmp->bag);
        while(index > 1)
        {
            niceTD* son = constructor(1, bagSize);
            copyArray(bagSize, tmp->bag, son->bag);
            son->bag[index-1] = 0;
            tmp->left = son;
            tmp = son;
            index = nextZero(bagSize, tmp->bag);
        }
        niceTD* end = constructor(0, bagSize);
        tmp->left = end;
    }
    free(toGo);
}

int* toVisit(int sizeSons, int sizeParcouru, int* sons, int* parcouru)
{
    int i;
    int index = 1;
    int* res = malloc(sizeof(int) * sizeSons+1);
    initArray(sizeSons, res);

    for(i = 0; i < sizeSons; i++)
    {
        if(sons[i] == 0) break;
        if(parcouru[sons[i]] == 0)
        {
            res[index] = sons[i];
            index++;
        }
    }

    res[0] = index;

    return res;
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

    free(tmp);

    return res;
}

int* put(int e, int size, int* tab)
{
    int i;
    int* res = malloc(sizeof(int) * size);

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
    int* tmp;
    if(sizeT1 == 0) return t2;
    if(sizeT2 == 0) return t1;
    if((t1[0] <= t2[0] && t1[0] != 0) || t2[0] == 0)
    {
        tmp = fusion(sizeT1-1, sizeT2, &t1[1], t2);
        return put(t1[0], sizeT1+sizeT2, tmp);
    }
    else
    {
        tmp = fusion(sizeT1, sizeT2-1, t1, &t2[1]);
        return put(t2[0], sizeT1+sizeT2, tmp);
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

int** cmpBags(int bagSize, int* b1, int* b2)
{
    int i;
    int indexI = 0;
    int indexF = 0;
    int** res = malloc(sizeof(int) * 2);
    int* introduce = malloc(sizeof(int) * bagSize);
    int* forget = malloc(sizeof(int) * bagSize);
    int* parcouru = malloc(sizeof(int) * bagSize);

    initArray(bagSize, introduce);
    initArray(bagSize, forget);
    initArray(bagSize, parcouru);

    for(i = 0; i < bagSize; i++)
    {
        if(dichotomie(b1[i], bagSize, 0, b2) != -1)
        {
            parcouru[b1[i]] = 1;
        }
        else
        {
            introduce[indexI] = b1[i];
            indexI++;
        }
    }

    for(i = 0; i < bagSize; i++)
    {
        if(parcouru[b2[i]] == 1) continue;
        forget[indexF] = b2[i];
        indexF++;
    }

    res[0] = introduce;
    res[1] = forget;

    free(parcouru);

    return res;
}

niceTD* constructor(int type, int size)
{
    niceTD* res = malloc(sizeof(niceTD));
    res->left = NULL;
    res->right = NULL;
    res->bag = malloc(sizeof(int) * size);
    initArray(size, res->bag);
    res->type = type;

    return res;
}

void print_ntd(niceTD* plop, int bagSize)
{

    if (plop==NULL)
    return;
    printf("type: %d  ----- ",plop->type);
    printArray(bagSize,plop->bag);

    printf("Left\n");
    print_ntd(plop->left, bagSize);
    printf("Right\n");
    print_ntd(plop->right, bagSize);
}
=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


// A utility function to swap two elements
int partition( int a[], int l, int r) {
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;

   while( 1)
   {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}

void _quickSort( int a[], int l, int r)
{
   int j;

   if( l < r )
   {
   	// divide and conquer
        j = partition( a, l, r);
       _quickSort( a, l, j-1);
       _quickSort( a, j+1, r);
   }

}

void quickSort(int arr[], const int size)
{
	_quickSort(arr, 0, size -1);
}

long bellNumber(int n)
{
   long bell[n+1][n+1];
   bell[0][0] = 1;
   for (int i=1; i<=n; i++)
   {
      // Explicitly fill for j = 0
      bell[i][0] = bell[i-1][i-1];

      // Fill for remaining values of j
      for (int j=1; j<=i; j++)
         bell[i][j] = bell[i-1][j-1] + bell[i][j-1];
   }
   return bell[n][0];
}

// Paul utils

int dichotomie(int value, int size, int res, int* tab)
{
    if(size == 1)
    {
        if(tab[0] == value) return res;
        return -1;
    }
    int i = size/2;
    if(tab[i] == value) return res+i;
    if(tab[i] > value || tab[i] == 0)
    {
        int* tmp = malloc(sizeof(int) * i);
        copyArray(i, tab, tmp);
        return dichotomie(value, i, res, tmp);
    }
    else
    {
        int* tmp = malloc(sizeof(int) * i+size%2);
        copyArray(i+size%2, &tab[i], tmp);
        return dichotomie(value, i+size%2, res+i, tmp);
    }
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

// optimized version
void copyArray(int size, int* t1, int* t2)
{
    memcpy(t2, t1, sizeof(int) * size);
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

// optimized version
void initArray(int size, int* array)
{
	memset(array, 0, sizeof(int) * size);
}



>>>>>>> 61dc2508e9d59633a8b47274063a30cc84ccb98d

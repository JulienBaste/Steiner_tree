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
        printf("%d : ", u);
        printArray(maxEdges, edgesTD[u]);
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

/*void cmpBag(int* b1, int* b2)
{

}

void memCopy(int destination[],const int source[],int n)
{
 for(int i=0;i<n;i++)
  destination[i]=source[i];
}

//depart,milieu et fin seront des indice de tableau
void fusion(int T[],int depart,int milieu,int fin)
{
    int cur1=depart,cur2=milieu,curTab=0;
    int tab[fin-depart+1];
    while(cur1 < milieu && cur2 <= fin)
    {
        if(T[cur1]<=T[cur2])
            tab[curTab]=T[cur1++];
        else
            tab[curTab]=T[cur2++];
        curTab++;
    }
    memCopy(&tab[curTab],&T[cur1],milieu-cur1);//on recopie le restant de la permière partie
    memCopy(&tab[curTab],&T[cur2],fin-cur2+1);//et de la deuxièpe partie

    memCopy(&T[depart],tab,fin-depart+1);//on met tout dans le tableau,fusion terminé !
}*/

/*void buildNiceTD(niceTD niceTD, int** bags, int** edges, int next)
{
    int* bag = bags[next];
}*/

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

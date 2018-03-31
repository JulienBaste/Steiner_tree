#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tGraph.h"
#include "utils.h"
#include "enum.h"


unsigned char* colorTable(tSolTable* t, int i)
{
    return t->colors + i * t->nbCol;
}

tSolTable* tSolTable_create(int bag[], const int size)
{
    tSolTable* t = malloc(sizeof(tSolTable));
    t->vertices  = malloc(sizeof(int) * size);
    copyArray(size, bag, t->vertices);
    t->nbCol   = size;
    t->nbLine  = bellNumber(size + 1);
    t->weights = malloc(sizeof(unsigned long) * t->nbLine);
    t->colors  = malloc(sizeof(unsigned char) * t->nbLine * t->nbCol);

    return t;

}

// private
char nextColor(tSolTable* t, int i)
{
    unsigned char* prevColors = colorTable(t,i-1);
    unsigned char* colors = colorTable(t,i);
    int tabSize = t->nbCol;

    memcpy(colors, prevColors, tabSize);
    // vertex to inc
    int Vt = tabSize - 1;
    // le prochain element à incrementer n'est pas le premier
    while(Vt != 0)
    {
        // la couleur du vertex = son second index
        if(colors[Vt] == Vt + 1)
        {
            colors[Vt] = 0; Vt = Vt - 1;
        }
        else
        {
            int potential = colors[Vt];
            char nc = colors[potential];
            while(nc <= colors[Vt]  && potential != Vt)
            {
                nc = colors[++potential];
            }
            if(potential == Vt) colors[Vt] = Vt + 1;
            else colors[Vt] = nc;
            break;
        }
    }

    if(Vt == 0 && colors[Vt] == 0)
    {
        colors[Vt] = 1; return 1;
    }

    return Vt;
}

void tSolTable_generate(tSolTable* t)
{
    // set the first line to 0, the first color
    memset(t->colors, 0, t->nbCol);
    int i = 0;
    while(nextColor(t, ++i));
}

void tSolTable_destroy(tSolTable* t)
{
    free(t->vertices);
    free(t->weights);
    free(t->colors);
    free(t);
}

void tSolTable_view(tSolTable* t)
{
    int i,j;
    for(i = 0; i < t->nbLine; i++)
    {
        printf("[");
        for(j = 0; j < t->nbCol; j++)
        {
            printf(" %d",colorTable(t,i)[j]);;
        }
        printf(" ]\n");
    }
}

void tSolTable_forget(tSolTable* t, int f)
{
    int i;
    int j;
    int index;
    unsigned char* current;
    unsigned char* tmp = malloc(sizeof(unsigned char) * t->nbCol);
    int* newVertices = malloc(sizeof(int) * t->nbCol-1);
    index = dichotomie(f, t->nbCol, 0, t->vertices);
    copyArray(index, t->vertices, newVertices);

    for(i = index; i < t->nbCol-1; i++)
    {
        newVertices[i] = t->vertices[i+1];
    }

    free(t->vertices);
    t->vertices = newVertices;

    for(i = 0; i < t->nbLine; i++)
    {
        current = colorTable(t,i);
        printf("current : %d\n", current[index]);
        if(current[index] == 0)
        {
            printf("Index : %d\n", index);
            printf("plop : %d\n", i);
            copyArray(t->nbCol, current, tmp);
            for(j = index; j < t->nbCol-1; j++)
            {
                current[j] = tmp[j+1];
            }
            current = tmp;
        }
        else
        {
            if(isLastColor(current, current[index], t->nbCol) > 1)
            {
                printf("salut : %d\n", i);
                copyArray(t->nbCol, current, tmp);
                for(j = index; j < t->nbCol-1; j++)
                {
                    current[j] = tmp[j+1];
                }
                current = tmp;
            }
            else
            {
                printf("coucou : %d\n", i);
                initArray(t->nbCol, current);
            }
        }
    }
}

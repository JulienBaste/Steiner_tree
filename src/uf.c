#include "uf.h"
#include <stdlib.h>
#include <string.h>

UF* UF_create(const int size)
{
    UF* uf = malloc(sizeof(UF));
    uf->parent =  malloc(sizeof(int) * (size + 1));
    uf->rank = malloc(sizeof(int) * (size + 1));
    uf->size = size;
    uf->nbClass = size;
    uf->cls = NULL;
    // init parents & ranks
    int i = 0;
    while(i <= size)
    {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        i++;
    }

    return uf;
}

UF* UF_clone(UF* uf)
{
    UF* nuf = (UF*) malloc(sizeof(UF));
    nuf->parent =  malloc(sizeof(int) * (uf->size + 1));
    nuf->rank = malloc(sizeof(int) * (uf->size + 1));
    memcpy(nuf->parent, uf->parent, sizeof(int) * (uf->size + 1));
    memcpy(nuf->rank, uf->rank, sizeof(int) * (uf->size + 1));
    nuf->size = uf->size;
    nuf->nbClass = uf->nbClass;
    nuf->cls = NULL;

    return nuf;
}

// bug mais pourquoi ?
void UF_destroy(UF* uf)
{
    free(uf->parent);
    free(uf->rank);
    //if(uf->cls != NULL) { free(uf->cls); }
    free(uf);
}

int  UF_find(UF* uf, int v)
{
    if(uf->parent[v] != v)
    {
        uf->parent[v] = UF_find(uf, uf->parent[v]);
    }
    return uf->parent[v];
}

void UF_cunion(UF* uf, int x, int y)
{
    if(x == y) return;

    int xroot = UF_find(uf, x);
    int yroot = UF_find(uf, y);

    if(xroot < yroot)
    {
        uf->parent[yroot] = xroot;
    }
    else
    {
        uf->parent[xroot] = yroot;
    }

    uf->nbClass--;
}

void UF_union(UF* uf, int x, int y)
{
    if(x == y) return;

    int xroot = UF_find(uf, x);
    int yroot = UF_find(uf, y);

    if(uf->rank[xroot] < uf->rank[yroot])
    {
        uf->parent[xroot] = yroot;
    }
    else
    {
        uf->parent[yroot] = xroot;

        if(uf->rank[xroot] == uf->rank[yroot])
        {
            uf->rank[xroot]++;
        }
    }

    uf->nbClass--;
}


void UF_buildClasses(UF* uf)
{
    if(uf->nbClass == 0) return;
    // tableau des classes d'équivalences
    uf->cls = malloc(sizeof(Class) * uf->nbClass);
    // tableau des valeurs rencontrer
    int* val = calloc(uf->size + 1, sizeof(int));

    // compteur de boucle
    int i;
    // compteur de classes
    int ci = 0;
    // pour la coleur d'un élément
    int c;

    for(i = 1; i <= uf->size; i++)
    {
        // la coleur de `i`, sa classe d'équivalence
        c = UF_find(uf, i); if(c == 0) continue;
        // aucune structure classe n'a encore été créer pour la coleur `c`
        if(val[c] == 0)
        {
            val[c] = ci + 1;

            uf->cls[ci].elements = malloc(sizeof(int) * uf->size);
            uf->cls[ci].root = c;
            uf->cls[ci].elements[uf->cls[ci].size++] = i;

            ci++;
        }
        else
        {
            uf->cls[val[c] - 1].elements[uf->cls[val[c] - 1].size++] = i;
        }
    }

    // à revoir
    for(i = 0; i < uf->nbClass; i++)
    {
        uf->cls[i].elements = realloc(uf->cls[i].elements, uf->cls[i].size * sizeof(int));
    }

    free(val);

}

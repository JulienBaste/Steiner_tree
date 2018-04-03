#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tGraph.h"
#include "utils.h"
#include "steiner.h"
#include "enum.h"
#include "uf.h"

#define Infinity LONG_MAX

unsigned char* tSolTable_colorTable(tSolTable* t, int i)
{
    return t->colors + i * t->nbCol;
}

tSolTable* tSolTable_create(int bag[], const int size)
{
    tSolTable* t = malloc(sizeof(tSolTable));
    t->vertices  = malloc(sizeof(int) * size);
    memcpy(t->vertices, bag, size * sizeof(int));
    t->nbCol   = size;
    t->nbLine  = bellNumber(size + 1);
    t->weights = malloc(sizeof(unsigned long) * t->nbLine);
    initWeights(t->weights, t->nbLine);
    t->colors  = malloc(sizeof(unsigned char) * t->nbLine * t->nbCol);
    t->index = NULL;

    return t;
}


tSolTable* tSolTable_introduce(tSolTable* childTab, int v, int bag[], tGraph* g, SteinerArgs args)
{
    // calculer la taille du Sac
    const int size = childTab->nbCol + 1;
    // créer la table du Sac ( allocation memoire )
    tSolTable* t = tSolTable_create(bag, size);
    // génerer les coleurs et un index pour la table
    tSolTable_generate(t);

    // pour chaque coleur du fils, génerer les coleurs du père

    // previous color
    unsigned char* pc = NULL;
    // new color
    unsigned char* nc = malloc(size);

    int  i, j, k, l; // for loops
    int  index; // index of a coloration
    int  vn; // neighbor of `v`
    int  vn_size; // edges of `v`
    int* vns; // neighbors of `v`
    UF*  nCps = NULL; // composantes induites par la coloration
    tGraph* sg = NULL;
    UF* cps = NULL;
    int* mask = NULL;
    tEdge* mst = NULL;
    int mst_size;
    long w;
    i = 0;

    while(i < childTab->nbLine)
    {
        pc = tSolTable_colorTable(t,i);

        // adapter `pc` pour `nc`
        for(k = 0; k < v - 1; k++)
        {
            nc[k] = pc[k];
        }
        for(k = v - 1; k < childTab->nbCol; k++)
        {
            if(pc[k] >= v) nc[k + 1] = pc[k] + 1;
            else
                nc[k + 1] = pc[k];
        }

        // le sous-graphe associer au Sac
        sg = tGraph_reduce(g, bag, size);
        // les différentes composantes selon la coloration `pc` dans `sg`
        cps = UF_create(size);
        for(k = 0; k < size; k++)
        {
            if(k == v - 1) continue;
            UF_cunion(cps, k+1, nc[k]);
        }
        // sommets adjacents à `v`
        vns = malloc(sizeof(int) * (sg->nodes - 1));
        vn_size = 0;
        For_Neighbor_Of(sg,v,vn)
        {
            vns[vn_size++] = vn;
        }
        vns = realloc(vns, vn_size * sizeof(int));
        // créer le mask
        mask = malloc(sizeof(int) * vn_size);
        // 1. cas ou aucune arêtes n'est prise
        init_mask(mask, vn_size);
        // 1.1 `v` ne fait pas partie de la solution
        nc[v - 1] = 0;
        index = tSolTable_indexOf(t,nc);
        if(arraySearch(args.terminals, args.nbTerminals, v))
        {
            t->weights[index] = Infinity;
        } else {
            t->weights[index] = childTab->weights[i];
        }
        // 1.2 `v` est tout seul dans sa composante
        nc[v - 1] = v;
        index = tSolTable_indexOf(t,nc);
        t->weights[index] = childTab->weights[i];
        // 2. les autres cas
        k = 0;
        while(k < (1 << vn_size) - 1)
        {
            next_mask(mask, vn_size);
            nCps = UF_clone(cps);
            for(j = 0; j < vn_size; j++)
            {
                if(mask[j] == 0) continue;
                UF_cunion(nCps, v, pc[vns[j] - 1]);
            }
            UF_buildClasses(nCps);
            for(j = 0; j < nCps->nbClass; j++)
            {
                if(nCps->cls[j].elements[0] == 0) continue;

                nc[v - 1] = nCps->cls[j].elements[0];
                index = tSolTable_indexOf(t,nc);

                if(tGraph_belongs_to_tree(sg, nCps->cls[j].elements, nCps->cls[j].size))
                {
                    mst = tGraph_mstOfNodes(sg,
                                            nCps->cls[j].elements,
                                            nCps->cls[j].size,
                                            nCps->cls[j].elements[nCps->cls[j].size - 1],
                                            &mst_size);
                    w = 0;
                    for(l = 0; l < mst_size; l++)
                    {
                        w += mst[l].val;
                    }
                    t->weights[index] = w;
                }
                else {
                    t->weights[index] = Infinity;
                }
            }
            UF_destroy(nCps);
        }

        free(vns);
        free(mask);
        UF_destroy(cps);
        tGraph_destroy(sg);
    }
    //

    tSolTable_destroy(childTab);
    return t;
}

int tSolTable_indexOf(tSolTable* t, unsigned char col[])
{
    const int n = t->nbCol;
    unsigned char* z = calloc(n + 1, sizeof(int));
    Itree* root = t->index;
    int i = 1;
    while(i <= n)
    {
        z[i] = z[i-1];
        root = root->childs[col[i - 1] - z[col[i - 1]]];
        if(col[i - 1] != i) ++z[i];
        i++;
    }
    return root->index;
}

Itree* generateColors(tSolTable* t, unsigned char* pc, unsigned char* color, int length, int depth, int* count)
{
    if(depth < t->nbCol)
    {
        Itree* node  = malloc(sizeof(Itree));
        node->childs = malloc(sizeof(Itree*) * (length + 1));
        int i;
        for(i = 0; i < length; i++)
        {
            color[depth] = pc[i];
            node->childs[i] = generateColors(t, pc, color, length, depth + 1, count);
        }
        color[depth] = depth + 1;
        pc[length] = depth + 1;
        node->childs[length] = generateColors(t, pc, color, length + 1, depth + 1, count);
        return node;
    }
    else if(depth == t->nbCol)
    {
        Itree* node = malloc(sizeof(Itree));
        node->index = (*count)++;
        memcpy(tSolTable_colorTable(t, node->index), color, t->nbCol);
        return node;
    }

    return NULL;
}


void tSolTable_generate(tSolTable* t)
{
    // stocker la coleur de chaque sommet
    unsigned char* color = malloc(sizeof(unsigned char) * t->nbCol);
    // les coleurs possibles
    unsigned char* pc = malloc(sizeof(unsigned char) * (t->nbCol + 1));
    // n'importe quel sommet peut prendre la coleur 0
    pc[0] = 0;
    // compteur de coleurs
    int cpt = 0;

    t->index = generateColors(t, pc, color, 1, 0, &cpt);

}

void tSolTable_destroy(tSolTable* t)
{
    free(t->vertices);
    free(t->weights);
    free(t->colors);
    free(t->index);
    free(t);
}

void tSolTable_view(tSolTable* t)
{
    int i,j;
    for(i = 0; i<t->nbLine; i++)
    {
        printf("at index %d ",tSolTable_indexOf(t,tSolTable_colorTable(t,i)));
        printf("[");
        for(j = 0; j<t->nbCol; j++)
        {
            printf(" %d",tSolTable_colorTable(t,i)[j]);;
        }
        printf(" ] weight : %ld\n", t->weights[i]);
    }
}

tSolTable* tSolTable_forget(tSolTable* t, int f)
{
    int i;
    int j;
    int indexNew;
    int index = dichotomie(f, t->nbCol, 0, t->vertices);
    int* newBag = malloc(sizeof(int) * t->nbCol - 1);
    unsigned char* current;
    unsigned char* newCurrent = malloc(sizeof(unsigned char) * t->nbCol - 1);
    tSolTable* newT;

    for(i = 0; i < t->nbCol - 1; i++)
    {
        if(i < index)
        {
            newBag[i] = t->vertices[i];
        }
        else
        {
            newBag[i] = t->vertices[i + 1];
        }
    }

    newT = tSolTable_create(newBag, t->nbCol - 1);
    tSolTable_generate(newT);

    for(i = 0; i < t->nbLine; i++)
    {
        current = tSolTable_colorTable(t, i);

        for(j = 0; j < newT->nbCol; j++)
        {
            if(j < index)
            {
                newCurrent[j] = current[j];
            }
            else
            {
                newCurrent[j] = current[j + 1];
            }
        }

        indexNew = tSolTable_indexOf(newT, newCurrent);

        if(isLast(current, newT->nbCol, current[index]))
        {
            newT->weights[indexNew] = Infinity;
        }
        else
        {
            if(t->weights[i] < newT->weights[indexNew])
            {
                newT->weights[indexNew] = t->weights[i];
            }
        }
    }

    free(newCurrent);
    free(newBag);
    tSolTable_destroy(t);
    return newT;
}

tSolTable* tSolTable_join(tSolTable* t1, tSolTable* t2)
{
    tSolTable* newT = tSolTable_create(t1->vertices, t1->nbCol);
    unsigned char* current = calloc(t1->nbCol, sizeof(unsigned char));
    unsigned char* currentT1;
    int** edgesT1;
    int** edgesT2;
    int* tmp;
    int i;
    int j;
    int k;
    int l;
    int m;
    int minimum;
    int index;

    for(i = 0; i < t1->nbLine; i++)
    {
        currentT1 = tSolTable_colorTable(t1, i);
        edgesT1 = tSolTable_cmp(currentT1, t1->nbCol);

        for(j = 0; j < t2->nbLine; j++)
        {
            current = memcpy(current, tSolTable_colorTable(t2, i), t2->nbCol);
            edgesT2 = tSolTable_cmp(current, t2->nbCol);
            k = 1;
            l = 1;

            while(k < t1->nbCol)
            {
                while(l < t1->nbCol)
                {
                    if(isConnected(edgesT1[k], edgesT2[l], t1->nbCol))
                    {
                        if(tmp != NULL) free(tmp);
                        tmp = mergeColor(edgesT1[k], edgesT2[l], t1->nbCol);
                        minimum = min(k, l);

                        for(m = 0; m < t1->nbCol; m++)
                        {
                            if(tmp[m] == -1) break;
                            current[tmp[m]] = minimum;
                        }
                        edgesT2 = tSolTable_cmp(current, t2->nbCol);
                    }
                    else l++;
                }
                k++;
            }

            for(k = 0; k < t1->nbCol; k++)
            {
                if(current[k] == 0 && currentT1[k] != 0) current[k] = currentT1[k];
            }

            index = tSolTable_indexOf(newT, current);

            if(newT->weights[index] > t1->weights[i] + t2->weights[j])
            {
                newT->weights[index] = t1->weights[i] + t2->weights[j];
            }
        }
    }

    free(current);
    free(tmp);
    tSolTable_destroy(t1);
    tSolTable_destroy(t2);
    return newT;
}

tSolTable* tSolTable_feuille()
{
    return tSolTable_create(NULL, 0);
}

int** tSolTable_cmp(unsigned char* c, int size)
{
    int i;
    int index;
    int** res = malloc(sizeof(int) * size);

    for(i = 0; i < size; i++)
    {
        res[i] = calloc(size, sizeof(int));
        initArrayWith(res[i], -1, size);
    }

    for(i = 0; i < size; i++)
    {
        index = nextThing(size, -1, res[c[i]]);
        res[c[i]][index] = i;
    }

    return res;
}

int* mergeColor(int* c1, int* c2, int size)
{
    int i;
    int index1 = 0;
    int index2 = 0;
    int* res = malloc(sizeof(int) * size);
    initArrayWith(res, -1, size);

    for(i = 0; i < size; i++)
    {
        if(c1[i - index1] == -1)
        {
            res[i] = c2[i - index2];
            continue;
        }
        if(c2[i - index2] == -1)
        {
            res[i] = c1[i - index1];
            continue;
        }

        if(c1[i - index1] < c2[i - index2])
        {
            res[i] = c1[i - index1];
            index2++;
        }
        else if(c1[i - index1] == c2[i - index2])
        {
            res[i] = c1[i - index1];
        }
        else
        {
            res[i] = c2[i - index2];
            index1++;
        }
    }

    return res;
}

int isConnected(int* c1, int* c2, int size)
{
    int tmp;
    int res = 0;
    int i;

    for(i = 0; i < size; i++)
    {
        tmp = dichotomie(c1[i], size, 0, c2);
        if(tmp != -1)
        {
            res = 1;
            break;
        }
    }

    return res;
}

int isFinished(int* t, int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        if(t[i] == -1) return 0;
    }

    return 1;
}

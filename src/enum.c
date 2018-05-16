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
#define DEBUG 0

#define Color(tab, c) tab[c - 1]

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


tSolTable* tSolTable_introduce(tSolTable* childTab, int v, int bag[], tGraph* g, SteinerArgs* args)
{
    if(DEBUG)
    {
        printf("tSolTable introduce vertex %d \n\n",v);
    }
    // calculer la taille du Sac
    const int bag_size = childTab->nbCol + 1;
    // créer la table du Sac ( allocation memoire )
    tSolTable* t = tSolTable_create(bag, bag_size);
    // génerer les coleurs et un index pour la table
    tSolTable_generate(t);

    // pour chaque coleur du fils, génerer les coleurs du père

    // previous color
    unsigned char* pc = NULL;
    // new color
    unsigned char* nc = malloc(bag_size * sizeof(unsigned char));

    int  i, j, k, l, c; // for loops
    int  index; // index of a coloration
    int  vn; // neighbor of `v`
    int  vn_size; // edges of `v`
    int* vns; // neighbors of `v`
    UF  *cps = NULL, *nCps = NULL; // composantes induites par la coloration
    tGraph* sg = NULL; // sous-garphe associé au sac
    tGraph* cg = NULL; // le sous-graphe associé à une composante
    int* mask = NULL; // masque utiliser pour la génération des combinaisons
    tEdge* mst = NULL;
    int mst_size;
    long w;
    int flag;

    i = 0;

    while(i < childTab->nbLine)
    {
        pc = tSolTable_colorTable(childTab,i);

        if(DEBUG)
        {
            printf("1. adaptation de l'ancienne coloration \n");
            printf("ancienne coloration : ");
            printCharArray(pc, childTab->nbCol);
        }

        // adapter `pc` pour `nc`
        for(c = 1; c != v; c++)
        {
            Color(nc, c) = Color(pc, c);
        }
        for(c = v; c <= childTab->nbCol; c++)
        {
            if(Color(pc, c) >= v) Color(nc, c+1) = Color(pc, c) + 1;
            else
                Color(nc, c+1) = Color(pc, c);
        }

        Color(nc , v) = v;

        if(DEBUG)
        {
            printf("nouvelle coloration : ");
            printCharArray(nc, bag_size);
            printf("\n\n");
        }


        // cas 1: `v` ne change pas les composantes

        // 1.1 `v` ne fait pas parti de la solution
        Color(nc, v) = 0;
        index = tSolTable_indexOf(t,nc);
        if(arraySearch(args->terminals, args->nbTerminals, v))
        {
            t->weights[index] = Infinity;
        } else {
            t->weights[index] = childTab->weights[i];
        }
        // 1.2 `v` est tout seul dans sa composante
        Color(nc, v) = v;
        index = tSolTable_indexOf(t,nc);
        t->weights[index] = childTab->weights[i];

        // cas 2. `v` change les composantes

        // le sous-graphe associer au Sac
        sg = tGraph_reduce(g, bag, bag_size);
        // les différentes composantes selon la coloration `pc` dans `sg`
        cps = UF_create(bag_size);
        for(c = 1; c <= bag_size; c++)
        {
            UF_cunion(cps, c, Color(nc, c));
        }

        if(DEBUG)
        {
            UF_buildClasses(cps);
            printf("la coloration: ");
            printCharArray(nc, bag_size);
            printf("à céer %d composantes \n", cps->nbClass);
            printf("liste des composantes: \n");
            for(l = 0; l < cps->nbClass; l++)
            {
                printf("class with root: %d \n",cps->cls[l].elements[0]);
                printf("class elements: ");
                printIntArray(cps->cls[l].elements, cps->cls[l].size);
                printf("\n");
            }
        }
        // toute les colorations possible de v
        for(c = 1; c != v; c++)
        {
            Color(nc, v) = Color(nc, c);
            if(Color(nc,v) == 0) continue;
            nCps = UF_clone(cps);
            UF_cunion(nCps, v, Color(nc, c));
            UF_buildClasses(nCps);
            index = tSolTable_indexOf(t,nc);
            t->weights[index] = 0;
            for(j = 0; j < nCps->nbClass; j++)
            {
                if(nCps->cls[j].elements[0] == 0) continue;

                cg = tGraph_reduce(sg, nCps->cls[j].elements, nCps->cls[j].size);

                if(tGraph_isTree(cg))
                {
                    mst = tGraph_mst(cg, &mst_size);
                    w = 0;
                    for(l = 0; l < mst_size; l++)
                    {
                        w += mst[l].val;
                    }
                    t->weights[index] += w;
                }
                else
                {
                    t->weights[index] = Infinity;
                    break;
                }
                tGraph_destroy(cg);
            }
        }

        // coloration induites par les sommets adjacents à `v`

        // liste des sommets adjacents à v
        vns = malloc(sizeof(int) * (sg->nodes - 1));
        vn_size = 0;
        flag = 1;
        For_Neighbor_Of(sg,v,vn)
        {
            // solution impossible, prendre l'arête d'un sommet non accépté dans la solution
            if(Color(nc, vn) == 0) { flag = 0; break;}
            vns[vn_size++] = vn;
        }

        if(flag)
        {
            vns = realloc(vns, vn_size * sizeof(int));
            // créer le mask
            mask = malloc(sizeof(int) * vn_size);
            // 1. cas ou aucune arêtes n'est prise
            init_mask(mask, vn_size);
            // 2. les autres cas
            k = 1;
            // toute les combinaisons possible
            while(k < 1 << vn_size)
            {
                next_mask(mask, vn_size);
                nCps = UF_clone(cps);
                for(j = 0; j < vn_size; j++)
                {
                    if(mask[j] == 0) continue;
                    UF_cunion(nCps, v, Color(pc, vns[j]));
                }
                UF_buildClasses(nCps);

                Color(nc, v) = UF_find(nCps, v);
                index = tSolTable_indexOf(t,nc);
                t->weights[index] = 0;

                for(j = 0; j < nCps->nbClass; j++)
                {
                    if(nCps->cls[j].elements[0] == 0) continue;

                    cg = tGraph_reduce(sg, nCps->cls[j].elements, nCps->cls[j].size);

                    if(tGraph_isTree(cg))
                    {
                        mst = tGraph_mst(cg, &mst_size);
                        w = 0;
                        for(l = 0; l < mst_size; l++)
                        {
                            w += mst[l].val;
                        }
                        t->weights[index] += w;
                    }
                    else
                    {
                        t->weights[index] = Infinity;
                        break;
                    }
                    tGraph_destroy(cg);
                }
                //UF_destroy(nCps);
                k++;
            }
            free(mask);
            //UF_destroy(cps);
        }
        free(vns);
        tGraph_destroy(sg);
        i++;
    }
    //

    //tSolTable_destroy(childTab);
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
    int index = recherche(f, t->nbCol, t->vertices);
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

        if(!isLast(current, newT->nbCol, current[index]))
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
    tSolTable_generate(newT);

    unsigned char* current = malloc(sizeof(unsigned char) * t1->nbCol);
    unsigned char* currentT1 = malloc(sizeof(unsigned char) * t1->nbCol);
    unsigned char* tmpc = malloc(sizeof(unsigned char) * t1->nbCol);
    int** edgesT1;
    int** edgesT2;
    int** toGo = malloc(sizeof(int*) * t1->nbCol*2);
    int* tmp;
    int i;
    int j;
    int k;
    int reminder;
    int minimum;
    int index;
    int nbToGo = 0;

    for(i = 0; i < t1->nbLine; i++)
    {
        for(j = 0; j < t2->nbLine; j++)
        {
            currentT1 = memcpy(currentT1, tSolTable_colorTable(t1, i), t2->nbCol);
            edgesT1 = tSolTable_cmp(currentT1, t1->nbCol);

            current = memcpy(current, tSolTable_colorTable(t2, j), t2->nbCol);
            edgesT2 = tSolTable_cmp(current, t2->nbCol);

            for(k = 1; k < t1->nbCol; k++)
            {
                if(edgesT1[k][1] != -1)
                {
                    toGo[nbToGo] = edgesT1[k];
                    nbToGo++;
                }
            }

            //printf("NbToGo : %d\n", nbToGo);

            if(nbToGo != 0)
            {
                reminder = nbToGo;

                for(k = 1; k < t1->nbCol; k++)
                {
                    if(edgesT2[k][1] != -1)
                    {
                        toGo[nbToGo] = edgesT2[k];
                        nbToGo++;
                    }
                }

                //printf("Reminder : %d\n", nbToGo);

                if(reminder == nbToGo)
                {
                    tmpc = memcpy(tmpc, current, t1->nbCol);
                    current = memcpy(current, currentT1, t1->nbCol);
                    currentT1 = memcpy(currentT1, tmpc, t1->nbCol);
                    nbToGo = 0;
                }
                else
                {
                    for(k = nbToGo; k < t1->nbCol*2; k++)
                    {
                        toGo[k] = NULL;
                    }

                    tmp = toGo[nbToGo - 1];
                    toGo[nbToGo - 1] = NULL;
                    nbToGo--;
                }
            }

            while(nbToGo != 0)
            {
                for(k = 0; k < t1->nbCol*2; k++)
                {
                    index = isConnected(tmp, toGo[k], t1->nbCol);
                    if(index != -1)
                    {
                        reminder = k;
                        break;
                    }
                }

                if(index != -1)
                {
                    minimum = min(current[tmp[index]], currentT1[toGo[reminder][index]]);
                    //printf("%d%d%d\n", current[0],current[1],current[2]);
                    tmp = mergeColor(tmp, toGo[reminder], t1->nbCol);

                    for(k = 0; k < t1->nbCol; k++)
                    {
                        if(tmp[k] == -1) continue;
                        current[tmp[k]] = minimum;
                    }

                    toGo[reminder] = NULL;
                    nbToGo--;
                }
                else
                {
                    for(k = 0; k < t1->nbCol*2; k++)
                    {
                        if(toGo[k] != NULL)
                        {
                            tmp = toGo[k];
                        }
                    }
                }
            }

            for(k = 0; k < t1->nbCol; k++)
            {
                if(current[k] == 0 && currentT1[k] != 0) current[k] = currentT1[k];
            }

            printf("%d %d %d\n", current[0], current[1], current[2]);

            index = tSolTable_indexOf(newT, current);

            if(newT->weights[index] > t1->weights[i] + t2->weights[j])
            {
                newT->weights[index] = t1->weights[i] + t2->weights[j];
            }

            for(k = 0; k < t1->nbCol; k++)
            {
                free(edgesT1[k]);
                free(edgesT2[k]);
            }

            free(edgesT1);
            free(edgesT2);
        }
        printf("FINI\n");
    }

    free(current);
    free(currentT1);
    free(tmpc);

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
    int index = 0;
    int** res = malloc(sizeof(int*) * (size + 1));

    for(i = 0; i < size + 1; i++)
    {
        res[i] = malloc(sizeof(int) * size);
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
    int i;

    for(i = 0; i < size; i++)
    {
        tmp = dichotomie(c1[i], size, 0, c2);
        if(tmp != -1)
        {
            return tmp;
        }
    }

    return -1;
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

tSolTable* tSolTable_computeSon(niceTD* ntd, SteinerArgs* args)
{
    tSolTable* left;
    tSolTable* right;
    tSolTable* res;

    switch(ntd->type)
    {
        case 0: return tSolTable_create(NULL, 0);
        case 1:
        {
            /*int** tmp;
            int v;*/
            left = tSolTable_computeSon(ntd->left, args);
            left->vertices = ntd->bag;
            /*tmp = cmpBags(left->nbCol, left->vertices, ntd->bag);
            v = tmp[0][0];
            res = tSolTable_introduce(left, v, ntd->bag, args->tg, args);
            free(tmp[0]);
            free(tmp[1]);
            free(tmp);
            tSolTable_destroy(left);
            res = left;
            break;
        }
        case 2:
        {
            int** tmp;
            left = tSolTable_computeSon(ntd->left, args);
            tmp = cmpBags(left->nbCol, left->vertices, ntd->bag);
            res = tSolTable_forget(left, tmp[0][0]);
            free(tmp[0]);
            free(tmp[1]);
            free(tmp);
            tSolTable_destroy(left);
            break;
        }
        case 3:
        {
            left = tSolTable_computeSon(ntd->left, args);
            right = tSolTable_computeSon(ntd->right, args);
            res = tSolTable_join(left, right);
            tSolTable_destroy(left);
            tSolTable_destroy(right);
            break;
        }
    }

    printIntArray(res->vertices, res->nbCol);

    return res;
}

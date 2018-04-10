#include <limits.h>
#include "tGraph.h"
#include "niceTD.h"
#include "steiner.h"
#include "enum.h"

tGraph* tGraph_steiner(tGraph* g, SteinerArgs args)
{
    return g;
}

int calculSteiner1(tGraph* g, SteinerArgs* args)
{
    int i;
    int min = MAX_INT;
    tSolTable* res = tSolTable_computeSon(args->ntd);

    for(i = 0; i < res->nbLine; i++)
    {
        if(min > res->weights[i])
        {
            res->weights[i] = min;
        }
    }

    return min;
}

tSolTable* tSolTable_computeSon(niceTD* ntd)
{
    tSolTable* left;
    tSolTable* right;
    tSolTable* res;

    switch(ntd->type)
    {
        case 0: return tSolTable_create(NULL, 0);
        case 1:
        {
            left = tSolTable_computeSon(ntd->left);
            break;
        }
        case 2:
        {
            int** tmp;
            int f;
            left = tSolTable_computeSon(ntd->left);
            tmp = cmpBags(left->nbCol, left->vertices, ntd->bag);
            f = tmp[1][0];
            res = tSolTable_forget(left, f);
            free(tmp[0]);
            free(tmp[1]);
            free(tmp);
            tSolTable_destroy(left);
            return res;
        }
        case 3:
        {
            left = tSolTable_computeSon(ntd->left);
            right = tSolTable_computeSon(ntd->right);
            res = tSolTable_join(left, right);
            tSolTable_destroy(left);
            tSolTable_destroy(right);
            return res;
        }
    }
}

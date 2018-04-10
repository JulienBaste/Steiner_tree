#include <limits.h>
#include "tGraph.h"
#include "niceTD.h"
#include "steiner.h"
#include "enum.h"

tGraph* tGraph_steiner(tGraph* g, SteinerArgs args)
{
    return g;
}

int calculSteiner1(SteinerArgs* args)
{
    int i;
    int min = INT_MAX;
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

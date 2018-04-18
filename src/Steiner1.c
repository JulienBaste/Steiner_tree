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
    tSolTable* res = tSolTable_computeSon(args->ntd, args);

    return res->weights[0];
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "tGraph.h"
#include "niceTD.h"
#include "steiner.h"
#include "enum.h"

int main(int argc, char const *argv[])
{
    SteinerArgs* args = preCalculs(argv[1]);
    ntd_debug(args->ntd, args->bagSize);
    //int res = calculSteiner1(args);

    int* b1 = malloc(sizeof(int) * 2);
    int* b2 = malloc(sizeof(int) * 3);

    b1[0] = 0; b2[1] = 1;
    b2[0] = 0; b2[1] = 1; b2[2] = 2;

    tSolTable* t = tSolTable_create(b1, 2);
    tSolTable_generate(t);

    tSolTable* res = tSolTable_introduce(t, 2, b2, args->tg, args);

    tSolTable_view(res);
}

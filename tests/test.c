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
    int* b1 = malloc(sizeof(int) * 3);
    int* b2 = malloc(sizeof(int) * 3);

    b1[0] = 51; b1[1] = 52; b1[2] = 53;
    b2[0] = 51; b2[1] = 52; b2[2] = 53;

    tSolTable* t1 = tSolTable_create(b1, 3);
    tSolTable* t2 = tSolTable_create(b2, 3);

    tSolTable* res = tSolTable_join(t1, t2);

    tSolTable_view(res);

    /*SteinerArgs* args = preCalculs(argv[1]);
    ntd_debug(args->ntd, args->bagSize);
    int res = calculSteiner1(args);

    printf("%d\n", res);*/
}

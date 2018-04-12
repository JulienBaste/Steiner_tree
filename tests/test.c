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

int main(int argc, char const *argv[])
{
    int** res;
    int* b1 = malloc(sizeof(int) * 3);
    int* b2 = malloc(sizeof(int) * 3);

    b1[0] = 51; b1[1] = 52; b1[2] = 53;
    b2[0] = 52; b2[1] = 53; b2[2] = 54;

    //printf("%d\n", dichotomie(53, 3, 0, b1));

    res = cmpBags(3, b1, b2);

    printf("Introduce : ");
    printIntArray(res[0], 3);
    printf("\nForget : ");
    printIntArray(res[1], 3);

    /*SteinerArgs* args = preCalculs(argv[1]);
    int res = calculSteiner1(args);
    printf("%d\n", res);*/
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "enum.h"

int main(int argc, char const *argv[])
{
    unsigned char tab1[]={1,1,0,0,0,0};
    unsigned char tab2[]={0,2,2,0,0,0};

    int** res1 = tSolTable_cmp(tab1, 6);
    int** res2 = tSolTable_cmp(tab2, 6);

    printIntArray(res1[1],6);
    printIntArray(res2[2],6);

    int* res = mergeColor(res1[1], res2[2], 6);

    printIntArray(res, 6);

}

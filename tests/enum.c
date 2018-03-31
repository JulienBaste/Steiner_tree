#include <stdlib.h>
#include <stdio.h>
#include "enum.h"


int main(int argc, char** argv)
{
    int bag[] = {22, 44};

    tSolTable* sols = tSolTable_create(bag, 2);
    tSolTable_generate(sols);
    tSolTable_view(sols);
    tSolTable_forget(sols, 44);
    printf("\n --------------- \n");
    tSolTable_view(sols);
    tSolTable_destroy(sols);

    return 0;
}

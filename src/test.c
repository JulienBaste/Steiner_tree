#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        perror("fopen");
    }

    int v1;
    int v2;
    int res = fscanf(file, "%d %d", &v1, &v2);
    printf("%d %d %d\n", res, v1, v2);
    return 0;
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    int test[0];
    int test2[25] = {0};
    printf("%d %d\n", sizeof(test)/sizeof(int), sizeof(test2)/sizeof(test2[0]));
    printArray(test2);
    return 0;
}

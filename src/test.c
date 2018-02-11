#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    int test[0];
    int test2[1];
    printf("%d %d\n", sizeof(test)/sizeof(int), sizeof(test2)/sizeof(int));
    return 0;
}

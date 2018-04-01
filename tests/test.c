#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    int tab[]={2,1,0,0};
    quickSort(tab, 4);
    printIntArray(tab, 4);
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    int tab[]={1,3,7,8,9,2,3,4,5,0,0};
    fusion(tab,0,5,10);
    printArray(11, tab);
    return 0;
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    int tab[]={9,8,7,0,6,5,4,0,3,2,1,0};
    int* res = malloc(sizeof(int) * 12);
    res = triFusion(12, tab);
    printArray(12, res);
    /*int* res = malloc(sizeof(int) * 1);
    res[0] = 0;
    if(res[0] == NULL)
    {
        printf("coucou\n");
    }
    else
    {
        printf("ocu\n");
    }
    return 0;*/
}

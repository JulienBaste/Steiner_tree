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
    SteinerArgs* args = preCalculs(argv[1]);
    int res = calculSteiner1(args);
    printf("%d\n", res);
}

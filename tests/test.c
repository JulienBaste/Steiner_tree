#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "niceTD.h"
#include "tGraph.h"
#include "steiner.h"

int main(int argc, char const *argv[])
{
    tGraph* tg = NULL;
    niceTD* ntd = NULL;

    preCalculs(argv[1], tg, ntd);
}

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "tGraph.h"
#include "niceTD.h"

int main(int argc, char const *argv[])
{
    SteinerArgs* res = preCalculs(argv[1]);

    ntd_debug(res->ntd, 5);
}

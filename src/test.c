#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

int main(int argc, char const *argv[])
{
    char *n = "01";
    char result[3];
    combination(n, 1, 0, result);
    return 0;
}

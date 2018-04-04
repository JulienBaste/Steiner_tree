#ifndef INC_NICETD_H_
#define INC_NICETD_H_

 #include <stdio.h>
#include "tGraph.h"

typedef struct niceTD niceTD;
struct niceTD
{
    niceTD* left;
    niceTD* right;
    int type;
    int* bag;

    /*  Type 0 = Feuille
        Type 1 = Introduce
        Type 2 = Forget
        Type 3 = Join
        In Type 1 and 2, the only son go on left
   */
};

typedef struct preCalcul preCalcul;
struct preCalcul
{
    niceTD* ntd;
    tGraph* tg;
};

#define BUFSIZE 1024

void  fillBags(FILE* file, int** bags, int nbBags, int bagSize);
int   fillEdgesTD(FILE* file, int** edgesTD, int maxEdges, int nbEdge);
void  initEdgesAtZero(int** edges, int first, int last);
void  buildNiceTD(niceTD* tree, int** bags, int** edges, int nbBags, int bagSize, int maxEdges, int next, int* parcouru);
int*  toVisit(int sizeSons, int sizeParcouru, int* sons, int* parcouru);
int*  findFirstTerminal(int** bags, int** edges, int bagSize, int maxEdges, int nbTerminals, int* terminals, int* parcouru, int next);
int** cmpBags(int bagSize, int* b1, int* b2);
void ntd_debug(niceTD* root, int bagSize);
niceTD* constructor(int type, int size);
preCalcul* preCalculs(const char* path);
#endif

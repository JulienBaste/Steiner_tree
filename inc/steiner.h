#ifndef STEINER_H_
#define STEINER_H_

#include "tGraph.h"
#include "niceTD.h"

tGraph* tGraph_steiner(tGraph* g, int terminals[], const int nTerminals, niceTD* ntd);

#endif

#ifndef STEINER_H_
#define STEINER_H_

#include "tGraph.h"
#include "niceTD.h"

typedef struct SteinerArgs SteinerArgs;

struct SteinerArgs {
	int*    terminals;
	int     nbTerminals;
	niceTD* ntd;
};

tGraph* tGraph_steiner(tGraph* g, SteinerArgs args);

#endif

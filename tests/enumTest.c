#include "enum.h"
#include "tGraph.h"
#include "amatrix.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	// init the algo
	int bag[] = {1};
	tSolTable* t = tSolTable_create(bag,1);
	tSolTable_generate(t);
	t->weights[0] = 0;
	t->weights[1] = 0;
	// load the graph
	tGraph* g = tGraph_from_file("res/home.gr");
	//
	SteinerArgs args;
	int terminals[] = {2,3,7};
	args.terminals = terminals;
	args.nbTerminals = 3;
	args.ntd = NULL;
	//
	int ibag1[] = {1, 2};
	tSolTable* it1 = tSolTable_introduce(t, 2, ibag1, g, args);
	//
	int ibag2[] = {1, 2, 3};
	tSolTable* it2 = tSolTable_introduce(it1, 3, ibag2, g, args);
	tSolTable_view(it2);


	tGraph_destroy(g);
	tSolTable_destroy(it1);
	tSolTable_destroy(t);

	return 0;
}

#include "tGraph.h"
#include "steiner.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	int terminals[] = { 2, 3, 7};
	const int size = 3;

	SteinerArgs args;
	args.terminals = terminals;
	args.nbTerminals = size;

	tGraph* g = tGraph_from_file("home.gr");
	tGraph* steiner = tGraph_steiner(g, args);
	tGraph_as_ost(steiner, "home.ost");

	tGraph_destroy(steiner);
	tGraph_destroy(g);
	return 0;
}

#include "tGraph.h"
#include "steiner.h"
#include "utils.h"
#include <limits.h>
#include <stdlib.h>

tGraph* tGraph_steiner(tGraph* g, SteinerArgs args)
{
	// arêtes du Graph G
	tEdge* edges = NULL;
	// pour stocker les sous-graphes de G
	tGraph* sg = NULL;
	// le Steiner trouver
	tGraph* steiner = NULL;
	// Mask: pour générer les combinaisons
	int *mask = NULL;
	//
	int size;
	long min_b, b;

	// le poids associer est infinity
	min_b = INT_MAX;
	// initialiser le masque utiliser pour générer les différents sous-graphe
	size = g->edges;
	mask =  malloc(sizeof(int) * size);
	init_mask(mask,size);
	next_mask(mask,size);
	// un sous-graphe au maximum contient g->nodes sommets
	sg = tGraph_create(g->nodes);
	edges = tGraph_edges(g);

	while(mask_contains(mask,size,1))
	{
		tGraph_associated_graph(g, edges, g->edges, mask, sg);
		if(tGraph_belongs_to_tree(sg, args.terminals, args.nbTerminals))
		{
			b = tGraph_weight(sg);
			if(b < min_b)
			{
				min_b = b;
				steiner = tGraph_copy(sg);
			}
		}
		next_mask(mask,size);
	}

	return steiner;
}




#include "tGraph.h"
#include "steiner.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <stdio.h>


// private operations
void init_mask(int mask[], const int size);
void next_mask(int mask[], const int size);
char mask_contains(int mask[], const int size, const int val);


tGraph* tGraph_steiner(tGraph* g, int terminals[], const int nTerminals, niceTD* ntd)
{
	// arêtes du Graph G
	tEdge* edges = NULL;
	// pour stocker les sous-graphes de G
	tGraph* sg = NULL;
	// le Steiner trouver
	tGraph* steiner = NULL;
	// Mask: pour générer les combinaisons
	int *mask = NULL;
	// Pour stocker le résultat de l'exploration d'un Graphe
	int *expl = NULL;
	int a, size;
	long min_b, b;

	// exploration à partir de 'a'
	a = terminals[0];
	// le poids associer est infinity
	min_b = INT_MAX;
	// initialiser le masque utiliser pour générer les différents sous-graphe
	size = g->edges;
	mask =  malloc(sizeof(int) * size);
	init_mask(mask,size);
	next_mask(mask,size);
	// un sous-graphe au maximum contient g->nodes sommets
	sg = tGraph_create(g->nodes);
	edges = tGraph_edges_of(g);
	// au maximum on explore tout les noeuds
	expl = malloc(sizeof(int) * g->nodes);

	while(mask_contains(mask,size,1))
	{
		tGraph_associated_graph(g, edges, mask, sg);
		tGraph_explore(sg, a, expl);
		if(tGraph_belongs_to_tree(sg, terminals, nTerminals))
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


// opérations sur le masque utiliser par l'algo

void inline init_mask(int mask[], const int size)
{
	memset(mask, 0, (sizeof(int) * size));
}

char mask_contains(int mask[], const int size, const int val)
{
	int i = 0;
	while(i<size)
	{
		if(mask[i] == val) return 1;
		i++;
	}
	return 0;
}

// k: la dernière case mise à zero
void next_mask(int mask[], const int size)
{
	int i = 0;
	while(i<size)
	{
		if(mask[i] == 1) mask[i] = 0;
		else
			{ mask[i] = 1; break; }
		i++;
	}
}

#include "tGraph.h"
#include "amatrix.h"
#include "utils.h"
#include "uf.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>


// private operations
void _sort(tEdge a[], int l, int r);
int  _partition(tEdge a[], int l, int r);




// création d'un graphe à n sommets
tGraph* tGraph_create(int nodes)
{
	tGraph* g = malloc(sizeof(tGraph));

	g->nodes = nodes;
	g->edges = 0;
	g->matrix = adja_matrix_create(nodes); adja_matrix_init(g->matrix);

	return g;
}

void tGraph_destroy(tGraph* g)
{
	free(g->matrix);
	free(g);
}

// génerer un graphe aléatoire
tGraph* tGraph_generate(int nodes)
{
	tGraph* g = tGraph_create(nodes);
	int   a,b, num_edges = 0;

	For_Possible_Edge(g,a,b) {
		Set_Edge(g,a,b, rand() % 1500);
		num_edges++;
	}
	g->edges = num_edges;

	return g;
}

// créer une copie du graphe
tGraph* tGraph_copy(tGraph* g)
{
	int a,b;
	tGraph* cg = tGraph_create(g->nodes);
	cg->edges = g->edges;

	For_Graph_Edge(g,a,b) Set_Edge(cg,a,b, Get_Edge(g,a,b));

	return cg;
}

long tGraph_weight(tGraph* g)
{
	int a,b;
	long w = 0;

	For_Graph_Edge(g,a,b)
		w += Get_Edge(g,a,b);


	return w;
}

tGraph* tGraph_from_file(const char* path)
{
	int nodes, edges, a, b , val;
	FILE* input = fopen(path,"r");
	if(input == NULL)
	{
		perror("fopen:");
		exit(EXIT_FAILURE);
	}
	tGraph* g = NULL;
	fscanf(input,"SECTION Graph\n");
	fscanf(input,"Nodes %d\n",&nodes); g = tGraph_create(nodes);
	fscanf(input,"Edges %d\n",&edges); g->edges = edges;
	while(fscanf(input,"E %d %d %d\n", &a, &b, &val))
	{
		Set_Edge(g,a,b,val);
	}
	fclose(input);

	return g;
}

void tGraph_as_file(tGraph* g, const char* path)
{
	FILE* out = fopen(path,"w"); if(out == NULL) { perror("fopen"); exit(EXIT_FAILURE); }
	int  a, b, val;
	//
	fprintf(out,"SECTION Graph\n");
	fprintf(out,"Nodes %d\n",g->nodes);
	fprintf(out,"Edges %d\n",g->edges);
	For_Graph_Edge(g,a,b)
	{
		val = Get_Edge(g,a,b);
		fprintf(out,"E %d %d %d\n",a,b,val);

	}
	fprintf(out,"END\n");

	fclose(out);
}

void tGraph_as_ost(tGraph* g, const char* path)
{
	FILE* out = fopen(path,"w");
	if(out == NULL)
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	int a,b;
	long w = tGraph_weight(g);
	fprintf(out,"VALUE %ld\n",w);
	For_Graph_Edge(g,a,b)
	{
		fprintf(out,"%d %d\n",a,b);
	}
	fclose(out);
}

tEdge* tGraph_edges_of(tGraph* g)
{
	int a,b,i;
	tEdge e;
	tEdge* t;

	t = malloc(sizeof(tEdge) * g->edges);
	i = 0;
	For_Graph_Edge(g,a,b)
	{
		e.x = a; e.y = b; e.val = Get_Edge(g,a,b);
		t[i] = e;
		i++;
	}

	return t;
}

// hypothèse: nodes[] est triée
tEdge* tGraph_edgesOfNodes(tGraph* g, int nodes[], const int size, int* nbEdges)
{
	int a, b, i; tEdge e; tEdge* edges; int ecount;

	edges = malloc(sizeof(tEdge) * size * size);
	ecount = 0;
	i = 0;
	while(i < size)
	{
		a = nodes[i];
		For_Succ_Of(g,a,b)
		{
			// a optimiser
			if(arraySearch(nodes, size, b))
			{
				e.x = a; e.y = b; e.val = Get_Edge(g,a,b);
				edges[ecount++] = e;
			}
		}
		i++;
	}
	edges = realloc(edges, ecount * sizeof(tEdge));
	*nbEdges = ecount;

	return edges;
}

tEdge* tGraph_mstOfNodes(tGraph* g, int nodes[], const int size, int maxVal, int* rSize)
{
	int nbEdges, i, k;
	tEdge e, *edges, *mst;
	UF* uf;

	edges = tGraph_edgesOfNodes(g, nodes, size, &nbEdges);
	_sort(edges, 0, nbEdges - 1);
	mst = malloc(sizeof(tEdge) * nbEdges);
	uf  = UF_create(maxVal);

	k = 0;
	for(i = 0; i<nbEdges; i++)
	{
		e = edges[i];
		if(UF_find(uf, e.x) != UF_find(uf, e.y))
		{
			mst[k++] = e;
			UF_union(uf, e.x, e.y);
		}
	}
	*rSize = k;
	mst = realloc(mst, k * sizeof(tEdge));

	free(edges);
	UF_destroy(uf);
	return mst;
}

void rec_explore(tGraph* g, int a, int b, int* v);

// parcourir le graphe à partir du noeud 'a'
// 'v' ensemble des noeuds visités
// hypothèse: size(v) == g->nodes
void tGraph_explore(tGraph* g, int a, int* v)
{
	int i;
	int  b;
	// init the table of visited nodes
	for(i=0;i<g->nodes;i++) v[i] = 0;
	// le noeud a est visité
	v[a-1] = 1;
	// visiter les successeurs de a
	For_Neighbor_Of(g,a,b) { rec_explore(g,b,a,v); }

}


void rec_explore(tGraph* g, int a, int b,int* v)
{
	int c;

	v[a-1] += 1;
	if(v[a-1] == 1) {
		For_Neighbor_Of(g,a,c) if(c != b) { rec_explore(g,c,a,v); }
	}
}


char tGraph_belongs_to_tree(tGraph* g, int nodes[], const int size)
{
	int i;
	// pour connaitre les noeuds visiter dans un parcours
	int* visited = malloc(sizeof(int) * g->nodes);
	memset(visited, 0, g->nodes * sizeof(int));
	// parcours
	tGraph_explore(g,nodes[0],visited);
	for(i = 0; i<size; i++)
	{
		if(visited[nodes[i] - 1] == 0 || visited[nodes[i] - 1] > 1) return 0;
	}
	return 1;
}

// graphe associer à un masque
void tGraph_associated_graph(tGraph* g, tEdge* edges, const int nbEdges,const  int* mask, tGraph* sg)
{
	int i;
	for(i=0; i<nbEdges; i++)
	{
		Set_Edge(sg, edges[i].x, edges[i].y, edges[i].val * mask[i]);
	}
}

tGraph* tGraph_reduce(tGraph*g, int nodes[], const int size)
{
	int a, b, k, val;
	tGraph* sg = tGraph_create(size);
	k = 0;
	For_Possible_Edge(sg,a,b)
	{
		val = Get_Edge(g, nodes[a-1], nodes[b-1]);
		Set_Edge(sg, a, b, val);
		if(val != 0) k++;
	}
	sg->edges = k;
	return sg;
}


// private operations


void _sort(tEdge a[], int l, int r)
{
   int j;

   if( l < r )
   {
   	// divide and conquer
        j = _partition( a, l, r);
       _sort( a, l, j-1);
       _sort( a, j+1, r);
   }
}

int _partition(tEdge a[], int l, int r) {

   int pivot, i, j;
   tEdge t;
   pivot = a[l].val;
   i = l; j = r+1;

   while(1)
   {
   	do ++i; while( a[i].val <= pivot && i <= r );
   	do --j; while( a[j].val > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}


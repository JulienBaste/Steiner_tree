#ifndef TGRAPH_H_
#define TGRAPH_H_

#include "amatrix.h"

// Macros sur le Graphe
#define Get_Edge(g,a,b) GET_Element(g->matrix,a,b)
#define Set_Edge(g,a,b,val) SET_Element(g->matrix,a,b,val)
// (a,b) arrête possible dans G, même si elle existe pas réelement
#define For_Possible_Edge(g,a,b) for(a=1; a<=g->nodes; a++) for(b=a+1; b<=g->nodes; b++)
// parcourir juste les arêtes qui existent(non null)
#define For_Graph_Edge(g,a,b) For_Possible_Edge(g,a,b)  if(Get_Edge(g,a,b) == 0) continue; else
// pour un 'a' fixé, 'b' est un voisin de a
#define For_Neighbor_Of(g,a,b) for(b=1; b<=g->nodes; b++) if(Get_Edge(g,a,b) == 0) continue; else
// pour un 'a' fixé, 'b' est un succ de a
#define For_Succ_Of(g,a,b) for(b=a+1; b<=g->nodes; b++) if(Get_Edge(g,a,b) == 0) continue; else

// data structures

typedef struct tGraph tGraph;
typedef struct tEdge tEdge;

struct tGraph {

	int    nodes;
	int    edges;
	int**  matrix;
};

struct tEdge {

	int x;
	int y;
	int val;
};

// operations

tGraph* tGraph_create(int nodes);
tGraph* tGraph_generate(int nodes);
tGraph* tGraph_copy(tGraph* g);
tGraph* tGraph_from_file(const char* path);
tGraph* tGraph_reduce(tGraph*g, int nodes[], const int size);
void tGraph_destroy(tGraph* g);
void tGraph_as_file(tGraph* g, const char* path);
void tGraph_as_ost(tGraph* g, const char* path);
long tGraph_weight(tGraph* g);
tEdge* tGraph_edges_of(tGraph* g);
void tGraph_explore(tGraph* g, int node, int* v);
char tGraph_belongs_to_tree(tGraph* g, int nodes[], const int size);
tEdge* tGraph_edgesOfNodes(tGraph* g, int nodes[], const int size, int* nbEdges);
tEdge* tGraph_mstOfNodes(tGraph* g, int nodes[], const int size, int maxVal, int* rSize);
void tGraph_associated_graph(tGraph* g, tEdge* edges, const int nbEdges,const  int* mask, tGraph* sg);




#endif

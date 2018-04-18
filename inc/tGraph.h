#ifndef TGRAPH_H_
#define TGRAPH_H_

#include "amatrix.h"

// Macros sur le Graphe

// Get the value of the Edge (a,b) on `g`
#define Get_Edge(g,a,b) GET_Element(g->matrix,(a),(b))
// Set the value of the Edge (a,b) on `g` to the value `val`
#define Set_Edge(g,a,b,val) SET_Element(g->matrix,(a),(b),(val))
// (a,b) arrête possible dans G, même si elle n'existe pas réelement
#define For_Possible_Edge(g,a,b) for(a=1; a<=g->nodes; a++) for(b=a+1; b<=g->nodes; b++)
// Parcourir juste les arêtes qui existent(non null)
#define For_Graph_Edge(g,a,b) For_Possible_Edge(g,a,b)  if(Get_Edge(g,a,b) == 0) continue; else
// Pour un noeud 'a' fixé, 'b' est un voisin de a
#define For_Neighbor_Of(g,a,b) for(b=1; b<=g->nodes; b++) if(Get_Edge(g,a,b) == 0) continue; else
// pour un 'a' fixé, 'b' est un succ de a(a < b)
#define For_Succ_Of(g,a,b) for(b=a+1; b<=g->nodes; b++) if(Get_Edge(g,a,b) == 0) continue; else

// data structures

typedef struct tGraph tGraph;
typedef struct tEdge tEdge;

struct tGraph {

	// le nombre de noeuds et d'arêtes
	int    nodes;
	int    edges;
	// la matrice d’adjacence
	int**  matrix;
};

struct tEdge {

	int x;
	int y;
	int val;
};

// operations

//	Allouer l’espace mémoire nécessaire à un graphe a `n` noeuds
tGraph* tGraph_create(int nodes);
//  Génération aléatoire d’un Graphe
tGraph* tGraph_generate(int nodes);
//	Faire une copie d’un Graphe
tGraph* tGraph_copy(tGraph* g);
//  Charger le Graphe à partir d'un fichier
tGraph* tGraph_from_file(const char* path);
/*	Générer le graphe réduit aux noeuds `nodes`
 *  nodes[i] dans `g` est associer au noeud (i + 1) dans `sg`  */
tGraph* tGraph_reduce(tGraph*g, int nodes[], const int size);
//  Libérer l’espace mémoire occupé par le graphe
void tGraph_destroy(tGraph* g);
//  Sérialiser le graphe
void tGraph_as_file(tGraph* g, const char* path);
//	Sérialiser le graphe selon le format OST
void tGraph_as_ost(tGraph* g, const char* path);
//  Calculer le poids du graphe
long tGraph_weight(tGraph* g);
//  Liste des arêtes du graphe
tEdge* tGraph_edges(tGraph* g);
/*  Explorer le graphe à partir du noeud `node`
 *  retourne un vecteur (V) avec la propriété suivante :
 *  V(node-1) = nombre de fois que `node` a été visité  */
int* tGraph_explore(tGraph* g, int node);
//  Dire si `g` est un arbre ou pas
char tGraph_isTree(tGraph* g);
//  Calculer le MST de `g`
tEdge* tGraph_mst(tGraph* g, int* mstSize);


// to delete

//  Retourne True(val <> 0) si le graphe réduit à `nodes` est un Arbre
char tGraph_belongs_to_tree(tGraph* g, int nodes[], const int size);
//  Retourne les arêtes associer aux noeuds nodes, avec nbEdges leur nombre
//  Hypothèse: nodes[] est triée
tEdge* tGraph_edgesOfNodes(tGraph* g, int nodes[], const int size, int* nbEdges);
//  Retourne le MST associer aux graphe réduit à nodes avec rSize sont nombre d'arêtes
tEdge* tGraph_mstOfNodes(tGraph* g, int nodes[], const int size, int maxVal, int* rSize);
//  Retourne
void tGraph_associated_graph(tGraph* g, tEdge* edges, const int nbEdges,const  int* mask, tGraph* sg);




#endif

#ifndef ENUM_H
#define ENUM_H

#include "tGraph.h"
#include "steiner.h"

typedef struct tSolTable tSolTable;

typedef union Itree Itree;

union Itree {

	int  index;
	Itree** childs;
};

struct tSolTable {

	// les sommets
	int*   vertices;
	// table des colorations
	unsigned char*  colors;
	// un index pour la table des colorations
	Itree* index;
	// pour chaque coloration associer un poids
	unsigned long* weights;
	// dimensions de la table des colorations
	int    nbCol;
	long   nbLine;

};

tSolTable* tSolTable_introduce(tSolTable* childTab, int v, int bag[], tGraph* g, SteinerArgs args);
tSolTable* tSolTable_create(int bag[], const int size);
unsigned char* tSolTable_colorTable(tSolTable* t, int i);
int  tSolTable_indexOf(tSolTable* t, unsigned char col[]);
void tSolTable_destroy(tSolTable* t);
void tSolTable_generate(tSolTable* t);
void tSolTable_view(tSolTable* t);

#endif

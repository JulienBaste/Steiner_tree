#ifndef ENUM_H
#define ENUM_H

typedef struct tSolTable tSolTable;


struct tSolTable {

	// les sommets
	int*   vertices;
	// table des colorations
	unsigned char*  colors;
	// pour chaque coloration associer un poids
	unsigned long* weights;
	// dimensions de la table des colorations
	int    nbCol;
	long   nbLine;

};

tSolTable* tSolTable_create(int bag[], const int size);
void tSolTable_destroy(tSolTable* t);
void tSolTable_generate(tSolTable* t);
void tSolTable_view(tSolTable* t);

#endif

#ifndef INC_UF_H_
#define INC_UF_H_

typedef struct UF UF;
typedef struct Class Class;

struct UF {

	int* parent;
	int* rank;
	int  size;
	int  nbClass;
	Class* cls;
};

struct Class {

	int* elements;
	int  root;
	int  size;
};


/* Allouer l’espace mémoire pour une structure union-find
 *  de valeur minimal  = 1 et valeur maximal = size */
UF*  UF_create(const int size);
// Faire une copie d'une structure Union-Find, les classes ne sont pas copier
UF*  UF_clone(UF* uf);
// Pour désallouer l’espace mémoire occupé
void UF_destroy(UF* uf);
/* Trouver la classe d'un élément v
   précondition: 1 <= v <= size */
int  UF_find(UF* uf, int v);
// Fusion de la classe de `x` avec celle de `y (version optimisé), 1 <= x,y <= size
void UF_union(UF* uf, int x, int y);
// Fusion de Classes de coleurs (utile pour l'algo de Steiner), 0 <= x,y <= size
void UF_cunion(UF* uf, int x, int y);
// Construire les classes d'équivalences
void UF_buildClasses(UF* uf);


#endif

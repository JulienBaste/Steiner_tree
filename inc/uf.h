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
	int  size;
};



UF*  UF_create(const int size);
UF*  UF_clone(UF* uf);
void UF_destroy(UF* uf);
int  UF_find(UF* uf, int v);
void UF_union(UF* uf, int x, int y);
void UF_cunion(UF* uf, int x, int y);
void UF_buildClasses(UF* uf);





#endif

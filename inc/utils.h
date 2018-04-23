#ifndef UTILS_H
#define UTILS_H

long bellNumber(int n);
void printIntArray(int arr[], const int size);
void quickSort(int arr[], const int size);
int  arraySearch(int array[], const int size, const int val);
int* triFusion(int size, int tab[]);
int* put(int e, int size, int* tab);
int* fusion(int sizeT1, int sizeT2, int* t1, int* t2);
int  dichotomie(int value, int size, int res, int* tab);
void init_mask(int mask[], const int size);
char mask_contains(int mask[], const int size, const int val);
void next_mask(int mask[], const int size);
int isLast(unsigned char* t, int size, unsigned char f);
void initWeights(unsigned long* w, int size);
int nextThing(int size, int v, int* array);
void initArrayWith(int* t, int v, int size);
int min(int i1, int i2);
void copyArray(int size, int* t1, int* t2);
void initArray(int size, int* array);
int nextZero(int size, int* array);
void printCharArray(unsigned char t[], const int size);
int recherche(int value, int size, int* tab);

#endif

#ifndef UTILS_H
#define UTILS_H

long bellNumber(int n);
void quickSort(int arr[], const int size);
void printIntArray(int arr[], const int size);
int intArrayCmp(int arr1[], int arr2[], const int size);
int  intArraySearch(int array[], const int size, const int val);
int* triFusion(int size, int tab[]);
int* put(int e, int size, int* tab);
int* fusion(int sizeT1, int sizeT2, int* t1, int* t2);
int  dichotomie(int value, int size, int res, int* tab);
// utiliser pour la génération de combinaison
void init_mask(int mask[], const int size);
char mask_contains(int mask[], const int size, const int val);
void next_mask(int mask[], const int size);
void printCharArray(unsigned char* arr, const int size);


#endif

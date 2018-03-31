#ifndef UTILS_H
#define UTILS_H

long bellNumber(int n);
void swap(int* a, int* b);
void initArray(int size, int* array);
void quickSort(int arr[], const int size);
int* triFusion(int size, int* tab);
void printArray(int size, int* t);
void copyArray(int size, int* t1, int* t2);
int* put(int e, int size, int* tab);
int* fusion(int sizeT1, int sizeT2, int* t1, int* t2);
int  dichotomie(int value, int size, int res, int* tab);
int isLastColor(unsigned char* color, unsigned char e, int size);

#endif

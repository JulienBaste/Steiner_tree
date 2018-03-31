#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// A utility function to swap two elements
int partition( int a[], int l, int r)
{
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;

   while(1)
   {
       if(pivot != 0)
       {
           do ++i; while(a[i] <= pivot && a[i] != 0 && i <= r);
           do --j; while(a[j] > pivot || a[j] == 0);
       } else
       {
           do ++i; while(i <= r);
           --j;
       }
       if( i >= j) break;
       t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}

void _quickSort( int a[], int l, int r)
{
   int j;

   if(l < r)
   {
       // divide and conquer
        j = partition( a, l, r);
       _quickSort( a, l, j-1);
       _quickSort( a, j+1, r);
   }

}

void quickSort(int arr[], const int size)
{
    _quickSort(arr, 0, size -1);
}

long bellNumber(int n)
{
   long bell[n+1][n+1];
   bell[0][0] = 1;
   for (int i=1; i<=n; i++)
   {
      // Explicitly fill for j = 0
      bell[i][0] = bell[i-1][i-1];

      // Fill for remaining values of j
      for (int j=1; j<=i; j++)
         bell[i][j] = bell[i-1][j-1] + bell[i][j-1];
   }
   return bell[n][0];
}

// Paul utils

int dichotomie(int value, int size, int res, int* tab)
{
    if(size == 1)
    {
        if(tab[0] == value) return res;
        return -1;
    }
    int i = size/2;
    if(tab[i] == value) return res+i;
    if(tab[i] > value || tab[i] == 0)
    {
        int* tmp = malloc(sizeof(int) * i);
        copyArray(i, tab, tmp);
        return dichotomie(value, i, res, tmp);
    }
    else
    {
        int* tmp = malloc(sizeof(int) * i+size%2);
        copyArray(i+size%2, &tab[i], tmp);
        return dichotomie(value, i+size%2, res+i, tmp);
    }
}


int* triFusion(int size, int* tab)
{
    if(size <= 1) return tab;
    else
    {
        int newSize = size/2;
        int* tmp = malloc(sizeof(int) * (newSize));
        copyArray(newSize, tab, tmp);
        return fusion(newSize, newSize+size%2, triFusion(newSize, tmp), triFusion(newSize+size%2, &tab[newSize]));
    }
}

int* fusion(int sizeT1, int sizeT2, int* t1, int* t2)
{
    if(sizeT1 == 0) return t2;
    if(sizeT2 == 0) return t1;
    if((t1[0] <= t2[0] && t1[0] != 0) || t2[0] == 0)
    {
        return put(t1[0], sizeT1+sizeT2, fusion(sizeT1-1, sizeT2, &t1[1], t2));
    }
    else
    {
        return put(t2[0], sizeT1+sizeT2, fusion(sizeT1, sizeT2-1, t1, &t2[1]));
    }
}

int* put(int e, int size, int* tab)
{
    int i;
    int* res = malloc(sizeof(int) * size+1);

    res[0] = e;
    for(i = 1; i <= size; i++)
    {
        res[i] = tab[i-1];
    }

    return res;
}

// optimized version
void copyArray(int size, int* t1, int* t2)
{
    memcpy(t2, t1, sizeof(int) * size);
}

void printArray(int size, int* t)
{
    int i;

    for(i = 0; i < size; i++)
    {
        printf("%d ", t[i]);
    }
    printf("\n");
}

// optimized version
void initArray(int size, int* array)
{
    memset(array, 0, sizeof(int) * size);
}

int isLastColor(unsigned char* color, unsigned char e, int size)
{
    int i;
    int compteur = 0;
    for(i = 0; i < size; i++)
    {
        if(color[i] == e)compteur++;
    }

    return compteur;
}

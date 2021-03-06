#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"



void inline init_mask(int mask[], const int size)
{
	memset(mask, 0, (sizeof(int) * size));
}

char mask_contains(int mask[], const int size, const int val)
{
	int i = 0;
	while(i<size)
	{
		if(mask[i] == val) return 1;
		i++;
	}
	return 0;
}

void next_mask(int mask[], const int size)
{
	int i = 0;
	while(i<size)
	{
		if(mask[i] == 1) mask[i] = 0;
		else
			{ mask[i] = 1; break; }
		i++;
	}
}

int intArraySearch(int array[], const int size, const int val)
{
	int i = 0;
	while(i < size) if(array[i++] == val) return 1;
	return 0;
}

// QuickSort

int partition( int a[], int l, int r) {
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;

   while( 1)
   {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}

void _quickSort( int a[], int l, int r)
{
   int j;

   if( l < r )
   {
   	// divide and conquer
        j = partition( a, l, r);
       _quickSort( a, l, j-1);
       _quickSort( a, j+1, r);
   }

}

void quickSort(int arr[], const int size)
{
	_quickSort(arr, 0, size - 1);
}


long bellNumber(int n)
{
   int i,j;

   long bell[n+1][n+1];
   bell[0][0] = 1;
   for (i=1; i<=n; i++)
   {
      // Explicitly fill for j = 0
      bell[i][0] = bell[i-1][i-1];

      // Fill for remaining values of j
      for (j=1; j<=i; j++)
         bell[i][j] = bell[i-1][j-1] + bell[i][j-1];
   }
   return bell[n][0];
}

void printIntArray(int t[], const int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        printf("%d ", t[i]);
    }
    printf("\n");
}

int intArrayCmp(int arr1[], int arr2[], const int size)
{
	int i;
	for(i=0; i<size; i++)
	{
		if(arr1[i] != arr2[i]) return 0;
	}
	return 1;
}

void printCharArray(unsigned char* arr, const int size)
{
	int i;
	printf("[");
	for(i = 0; i < size; i++)
	{
		printf(" %d", arr[i]);
	}
	printf(" ]\n");
}

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
        memcpy(tmp, tab, i * sizeof(int));
        return dichotomie(value, i, res, tmp);
    }
    else
    {
        int* tmp = malloc(sizeof(int) * i+size%2);
        memcpy(tmp, &tab[i], (i+size%2) * sizeof(int));
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
        memcpy(tmp, tab, newSize * sizeof(int));
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



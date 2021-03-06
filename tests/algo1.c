#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "niceTD.h"
#include "tGraph.h"
#include "steiner.h"

#define BUFSIZE 1024


void  fillBags(FILE* file, int** bags, int nbBags, int bagSize);
int   fillEdgesTD(FILE* file, int** edgesTD, int maxEdges, int nbEdge);
int   nextZero(int size, int* array);
void  initEdgesAtZero(int** edges, int first, int last);
void  buildNiceTD(niceTD* tree, int** bags, int** edges, int nbBags, int bagSize, int maxEdges, int next, int* parcouru);
int*  toVisit(int sizeSons, int sizeParcouru, int* sons, int* parcouru);
int*  findFirstTerminal(int** bags, int** edges, int bagSize, int maxEdges, int nbTerminals, int* terminals, int* parcouru, int next);
int** cmpBags(int bagSize, int* b1, int* b2);
void ntd_debug(niceTD* root, int bagSize);
niceTD* constructor(int type, int size);


int main(int argc, char** argv)
{
	/* Data */
	//
	tGraph* g = NULL;
	int maxEdges = 1;
	int a; // Edge.a
	int b; // Edge.b
	int i; // loop index
	int tmp_val;
    int nbTerminals;
    int nbEdges;
    int nbBags;
    int nbNodes;
    int bagSize;
    char buffer[BUFSIZE];
    // dynamicaly allocated
    FILE* input;
    int*  da_terminals;
    int** da_bags;
    int**  da_edgesTD;
    int*  da_parcouru;
    int*  da_res;


    if(argc < 2)
    {
    	fprintf(stderr,"missing argument, going to die :( \n"); exit(1);
    }

    // init the buffer, but why ?
    memset(buffer, '\0', BUFSIZE);

    input = fopen(argv[1], "r");

    if(input == NULL)
    {
        perror("fopen: "); exit(2);
    }

    // 1. Read the graph and store the result in `g`
	fscanf(input,"SECTION Graph\n");
	fscanf(input,"Nodes %d\n",&nbNodes); g = tGraph_create(nbNodes);
	fscanf(input,"Edges %d\n",&nbEdges); g->edges = nbEdges;
	while(fscanf(input,"E %d %d %d\n", &a, &b, &tmp_val))
	{
		Set_Edge(g,a,b,tmp_val);
	}
	fscanf(input,"ND\n\n");

	// 2. Read terminals and store them in `terminals`
	fscanf(input,"SECTION Terminals\n");
	fscanf(input,"Terminals %d\n",&nbTerminals);
    da_terminals = malloc(sizeof(int) * nbTerminals);
    i = 0;
    while(fscanf(input,"T %d\n",&tmp_val))
    {
    	da_terminals[i] = tmp_val;
    	i++;
    }
    fscanf(input,"END\n\n");

    quickSort(da_terminals ,nbTerminals);

    // 3. the Tree Decomposition
    fscanf(input,"SECTION Tree Decomposition\n");
    // cas des dernières instances, si on arrive :)
    if(fgetc(input) == 'c')
    {
        fgets(buffer, BUFSIZE, input);
    }

   fscanf(input," td %d %d %d\n", &nbBags, &bagSize, &tmp_val);

    da_bags = malloc(sizeof(int*) * (nbBags + 1));

    fillBags(input, da_bags, nbBags, bagSize);

    da_edgesTD = malloc((nbBags + 1) * sizeof(int));
    initEdgesAtZero(da_edgesTD, 0, nbBags + 1);
    maxEdges = fillEdgesTD(input, da_edgesTD, maxEdges, nbBags + 1);

    for(i = 1; i < nbBags + 1; i++)
    {
        quickSort(da_edgesTD[i], maxEdges);
    }

    da_parcouru = calloc(sizeof(int), nbBags + 2);

    da_res = findFirstTerminal(da_bags, da_edgesTD, nbBags, maxEdges, nbTerminals, da_terminals, da_parcouru, 1);

    niceTD* root = constructor(2, bagSize);
    initArray(bagSize, root->bag);
    initArray(nbBags+2, da_parcouru);
    root->bag[0] = da_bags[da_res[0]][da_res[1]];

    buildNiceTD(root, da_bags, da_edgesTD, nbBags, bagSize, maxEdges, da_res[0], da_parcouru);


    ntd_debug(root, bagSize);
    fclose(input);


    /*
    free(da_terminals);
    free(da_bags);
    free(da_edgesTD);
    free(da_parcouru);
    free(da_res);
	*/

    return 0;
}

void _ntd_debug(niceTD* root, int bagSize, int prof)
{
	if(root != NULL)
	{
		int i;
		for(i = 0; i<prof; i++)
		{
			printf("-");
		}
		switch(root->type)
		{
			case 0: printf("Leaf "); break;
			case 1: printf("Introduce "); break;
			case 2: printf("Forget "); break;
			case 3: printf("Join "); break;
			default: printf("Unknown ");
		}
		printf("[");
		if(root->type != 0)
		{
			for(i = 0; i<bagSize; i++)
			{
				if(root->bag[i] != 0)
					printf(" %d",root->bag[i]);
				else
					break;
			}
		}
		printf(" ]\n");
		fflush(stdout);
		// rec call
		_ntd_debug(root->left, bagSize, prof + 1);
		_ntd_debug(root->right, bagSize, prof + 1);
	}
}

void ntd_debug(niceTD* root, int bagSize)
{
	_ntd_debug(root, bagSize, 0);
}

void fillBags(FILE* file, int** bags, int nbBags, int bagSize)
{
    int i;
    int idBag;
    int size;

    for(i = 0; i < nbBags; i++)
    {
        size = 0;
        fscanf(file, "b %d", &idBag);
        bags[idBag] = calloc(sizeof(int), bagSize);
        while(fgetc(file) == ' ')
        {
            fscanf(file, "%d", &bags[idBag][size]);
            size++;
        }
        quickSort(bags[idBag],size);
    }
}

int fillEdgesTD(FILE* file, int** edgesTD, int maxEdges, int nbEdge)
{
    int i;
    int u;
    int v;
    int index;

    while(fscanf(file, "%d %d", &u, &v) > 0)
    {
        index = nextZero(maxEdges, edgesTD[u]);
        if(index == -1)
        {
            maxEdges = maxEdges * 2;
            for(i = 0; i < nbEdge; i++)
            {
                int* newEdge = malloc(sizeof(int) * maxEdges);
                initArray(maxEdges, newEdge);
                copyArray(maxEdges/2, edgesTD[i], newEdge);
                if(i == u)
                {
                    newEdge[(maxEdges/2)] = v;
                }
                edgesTD[i] = newEdge;
            }
        }
        else
        {
            edgesTD[u][index] = v;
        }
        index = nextZero(maxEdges, edgesTD[v]);
        edgesTD[v][index] = u;
    }

    return maxEdges;
}

int nextZero(int size, int* array)
{
    int i;

    for(i = 0; i < size; i++)
    {
        if(array[i] == 0) return i;
    }

    return -1;
}

void initEdgesAtZero(int** edges, int first, int last)
{
    int i;

    for(i = first; i <= last; i++)
    {
        edges[i] = malloc(sizeof(int) * 1);
        initArray(1, edges[i]);
    }
}


void buildNiceTD(niceTD* tree, int** bags, int** edges, int nbBags, int bagSize, int maxEdges, int next, int* parcouru)
{
    int i;
    int index;
    int newNext;
    int* toGo;
    int* bag = bags[next];
    int* fils = edges[next];
    int** newNode = cmpBags(bagSize, tree->bag, bag);
    niceTD* tmp = tree;
    int* tmp2;

    for(i = 0; i < bagSize; i++)
    {
        if(newNode[0][i] == 0) break;
        niceTD* son = constructor(1, bagSize);
        copyArray(bagSize, tmp->bag, son->bag);
        index = dichotomie(newNode[0][i], bagSize, 0, son->bag);
        son->bag[index] = 0;
        tmp2 = triFusion(bagSize, son->bag);
        free(son->bag);
        son->bag = tmp2;
        tmp->left = son;
        tmp = son;
    }

    for(i = 0; i < bagSize; i++)
    {
        if(newNode[1][i] == 0) break;
        niceTD* son = constructor(2, bagSize);
        copyArray(bagSize, tmp->bag, son->bag);
        index = nextZero(bagSize, son->bag);
        son->bag[index] = newNode[1][i];
        tmp2 = triFusion(bagSize, son->bag);
        free(son->bag);
        son->bag = tmp2;
        tmp->left = son;
        tmp = son;
    }

    parcouru[next] = 1;
    toGo = toVisit(maxEdges, nbBags+1, fils, parcouru);

    while(toGo[0] > 2)
    {
        niceTD* leftSon = constructor(2, bagSize);
        niceTD* rightSon = constructor(2, bagSize);
        copyArray(bagSize, tmp->bag, leftSon->bag);
        copyArray(bagSize, tmp->bag, rightSon->bag);
        tmp->left = leftSon;
        tmp->right = rightSon;
        tmp->type = 2;

        index = nextZero(maxEdges+1, toGo);
        newNext = toGo[index-1];
        toGo[index-1] = 0;
        toGo[0] = toGo[0] - 1;

        tmp = rightSon;
        buildNiceTD(leftSon, bags, edges, nbBags, bagSize, maxEdges, newNext, parcouru);
    }

    if(toGo[0] == 2)
    {
        buildNiceTD(tmp, bags, edges, nbBags, bagSize, maxEdges, toGo[1], parcouru);
    }

    if(toGo[0] == 1)
    {
        index = nextZero(bagSize, tmp->bag);
        while(index > 0)
        {
            niceTD* son = constructor(1, bagSize);
            copyArray(bagSize, tmp->bag, son->bag);
            index = nextZero(bagSize, son->bag);
            son->bag[index-1] = 0;
            tmp->left = son;
            tmp = son;
        }
        niceTD* end = constructor(0, 0);
        tmp->left = end;
    }
}

int* toVisit(int sizeSons, int sizeParcouru, int* sons, int* parcouru)
{
    int i;
    int index = 1;
    int* res = malloc(sizeof(int) * sizeSons+1);
    initArray(sizeSons, res);

    for(i = 0; i < sizeSons; i++)
    {
        if(sons[i] == 0) break;
        if(parcouru[sons[i]] == 0)
        {
            res[index] = sons[i];
            index++;
        }
    }

    res[0] = index;

    return res;
}

int* findFirstTerminal(int** bags, int** edges, int bagSize, int maxEdges, int nbTerminals, int* terminals, int* parcouru, int next)
{
    int i;
    int* bag = bags[next];
    int* res = malloc(sizeof(int) * 2);
    int* tmp = malloc(sizeof(int) * 2);

    for(i = 0; i < bagSize; i++)
    {
        if(bag[i] == 0) break;
        if(dichotomie(bag[i], nbTerminals, 0, terminals) != -1)
        {
            res = malloc(sizeof(int) * 2);
            res[0] = next;
            res[1] = i;
            return res;
        }
    }

    int* fils = edges[next];
    parcouru[next] = 1;
    res[0] = maxEdges;
    res[1] = bagSize;

    for(i = 0; i < maxEdges; i++)
    {
        if(fils[i] != 0 && parcouru[fils[i]] == 0)
        {
            tmp = findFirstTerminal(bags, edges, bagSize, maxEdges, nbTerminals, terminals, parcouru, fils[i]);
            if(tmp[0] < res[0])
            {
                res = tmp;
            }
            else if(tmp[0] == res[0] && tmp[1] < res[1])
            {
                res = tmp;
            }
        }
    }

    return res;
}

int** cmpBags(int bagSize, int* b1, int* b2)
{
    int i;
    int indexI = 0;
    int indexF = 0;
    int** res = malloc(sizeof(int) * 2);
    int* introduce = malloc(sizeof(int) * bagSize);
    int* forget = malloc(sizeof(int) * bagSize);
    int* parcouru = malloc(sizeof(int) * bagSize);

    initArray(bagSize, introduce);
    initArray(bagSize, forget);
    initArray(bagSize, parcouru);

    for(i = 0; i < bagSize; i++)
    {
        if(dichotomie(b1[i], bagSize, 0, b2) != -1)
        {
            parcouru[b1[i]] = 1;
        }
        else
        {
            introduce[indexI] = b1[i];
            indexI++;
        }
    }

    for(i = 0; i < bagSize; i++)
    {
        if(parcouru[b2[i]] == 1) continue;
        forget[indexF] = b2[i];
        indexF++;
    }

    res[0] = introduce;
    res[1] = forget;

    return res;
}

niceTD* constructor(int type, int size)
{
    niceTD* res = malloc(sizeof(niceTD));
    res->left = NULL;
    res->right = NULL;
    res->bag = malloc(sizeof(int) * size);
    res->type = type;

    return res;
}

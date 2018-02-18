void initMatrix(int size, int matrix[][size]);
void initArray(int size, int* array);
void displayMatrix(int size, int matrix[][size]);
void createEdges(int size, int matrix[][size], int u, int v, int w);
int ifEdgesExist(int size, int matrix[][size], int u, int v);
void deleteEdges(int size, int matrix[][size], int u, int v);
void createRandomMatrix(int nbNodes, int nbEdges, int matrix[][nbNodes]);
int* createTerminals(int nbTerminals);
void getNbNodes(FILE* file, int* nbNodes);
void getNbEdges(FILE* file, int* nbEdges);
void fillMatrix(FILE* file, int nbNodes, int nbEdges, int matrix[][nbNodes]);
void fillBags(FILE* file, int** bags, int nbBags, int bagSize);
void fillEdgesTD(FILE* file, int** edgesTD);
void initEdgesAtZero(int** edges, int first, int last);
void copyArray(int* t1, int* t2);
void printArray(int size, int* t);

typedef struct niceTD niceTD;
struct niceTD
{
    niceTD* left;
    niceTD* right;
    int type;
    int* bag;
    /*  Type 0 = Feuille
        Type 1 = Introduce
        Type 2 = Forget
        Type 3 = Join

        In Type 1 and 2, the only son go on left*/
};

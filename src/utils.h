void initMatrix(int size, int matrix[][size]);
void displayMatrix(int size, int matrix[][size]);
void createEdges(int size, int matrix[][size], int u, int v, int w);
int ifEdgesExist(int size, int matrix[][size], int u, int v);
void deleteEdges(int size, int matrix[][size], int u, int v);
void createRandomMatrix(int nbNodes, int nbEdges, int matrix[][nbNodes]);
int* createTerminals(int nbTerminals);
void getNbNodes(FILE* file, int* nbNodes);
void getNbEdges(FILE* file, int* nbEdges);
void fillMatrix(FILE* file, int nbNodes, int nbEdges, int matrix[][nbNodes]);
void fillBags(FILE* file, int** bags, int nbBags);
void fillEdgesTD(FILE* file, int** edgesTD);
void initEdges(int** edges, int first, int last);

typedef struct niceTD niceTD;
struct niceTD
{
    niceTD* filsGauche;
    niceTD* filsDroit;
    int type;
    int* bag;
};

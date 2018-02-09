void initMatrix(int size, int matrix[][size]);
void displayMatrix(int size, int matrix[][size]);
void createEdges(int size, int matrix[][size], int u, int v, int w);
int ifEdgesExist(int size, int matrix[][size], int u, int v);
void deleteEdges(int size, int matrix[][size], int u, int v);
void createRandomMatrix(int nbNodes, int nbEdges, int matrix[][nbNodes]);
int* createTerminals(int nbTerminals);
void combination(char n[], int k, int startPosition, char result[]);

typedef struct niceTD niceTD;
struct niceTD
{
    int type;
    int bag[];
    niceTD* filsGauche;
    niceTD* filsDroit;
}
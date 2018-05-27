#include "amatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Créer une matrice d'adjacence pour n sommets
int** adja_matrix_create(int n)
{
    int** mat;

    // Créer les lignes, chaque ligne est associer à un sommet ( mat[s] : table du sommet s )
    // le +1 pour avoir une case ou stocker la taille de matrice, case mat[0][0]
    mat = malloc(sizeof(int*) * (n + 1) );

    int s; // Le sommet s
    int table_size = n; // Taille de la table de s

    // Pour stocker la taille de la matrice
    mat[0] = malloc(sizeof(int));

    // Associer une table pour chaque case d'un sommet
    for(s=1; s<=n; s++)
    {
        mat[s] = malloc(sizeof(int) * table_size);
        table_size--;
    }

    // Taille de la matrice
    Length(mat) = n;

    return mat;
}


// Initialiser les cases d'une matrice d'adjacence à 0
// a optimiser
inline void adja_matrix_init(int** mat)
{
    int i,j;
    For_Matrix_Index(mat,i,j)
            mat[i][j] = 0;
}

// Afficher le contenu d'une matrice d'adjacence
void adja_matrix_view(int** mat)
{
    int i; int j;

    printf("[\n");
    For_Matrix_Line(mat,i) {
        printf("[ ");
        For_Line_Case(mat,i,j)
                printf("%d ", mat[i][j]);

        printf("]\n");
    }
    printf("]\n");
}

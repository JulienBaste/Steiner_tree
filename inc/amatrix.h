#ifndef AMATRIX_H_
#define AMATRIX_H_

// Macros sur la Matrice d'adjacence

// retourne la taille de la matrice
#define Length(m) m[0][0]
// pour parcourir la matrice ligne par ligne
#define For_Matrix_Line(m,i)  for(i=1; i<=Length(m); i++)
// pour un 'i' fixé parcourir une ligne case par case
#define For_Line_Case(m,i,j)  for(j=0; j<Length(m)-i+1; j++)
// pour parcourir la matrice case par case
#define For_Matrix_Index(m,i,j) For_Matrix_Line(m,i) For_Line_Case(m,i,j)
// récuperer la valeur de l'arête (a,b)
#define GET_Element(m,a,b) (a < b ? m[a][b-a] : m[b][a-b])
// affecter une valeur à l'arête (a,b)
#define SET_Element(m,a,b,val) if(a<b) m[a][b-a] = val; else m[b][a-b] = val;


// Opérations sur la matrice d'adjacence

int** adja_matrix_create(int n);
void  adja_matrix_init(int** mat);
void  adja_matrix_view(int** mat);



#endif

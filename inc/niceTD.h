#ifndef INC_NICETD_H_
#define INC_NICETD_H_

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
        In Type 1 and 2, the only son go on left
        */
};

#endif

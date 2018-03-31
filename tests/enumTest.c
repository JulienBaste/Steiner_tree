#include "enum.h"
#include <stdio.h>

int main(int argc, char** argv)
{

	int bag[] = {22, 23, 44, 55};

	tSolTable* sols = tSolTable_create(bag, 4);
	tSolTable_generate(sols);
	tSolTable_view(sols);
	tSolTable_destroy(sols);

	return 0;
}

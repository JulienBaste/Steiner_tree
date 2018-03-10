#include "enum.h"


int main(int argc, char** argv)
{
	int bag[] = {22, 23, 44};

	tSolTable* sols = tSolTable_create(bag, 3);
	tSolTable_generate(sols);
	tSolTable_view(sols);
	tSolTable_destroy(sols);

	return 0;
}


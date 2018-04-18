#include "tGraph.h"
#include "amatrix.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define FILE_PATH "res/home.gr"

// global variables
	tGraph* g;


int init_suite(void)
{
	g = tGraph_from_file(FILE_PATH);
	return 0;
}

int clean_suite(void)
{
	tGraph_destroy(g);
	return 0;
}

void tGraph_reduce_test(void)
{
	int nodes[] = {1, 3, 4, 7};
	const int NB_NODES = 4;
	int nbEdges = 0;
	int a,b, val;

	tGraph* sg = tGraph_reduce(g, nodes, NB_NODES);
	for(a = 0; a < NB_NODES; a++)
		for(b = a + 1; b < NB_NODES; b++)
		{
			val = Get_Edge(g, nodes[a], nodes[b]);
			CU_ASSERT_EQUAL(Get_Edge(sg, a+1, b+1), val);
			if(val != 0) nbEdges++;
		}
	CU_ASSERT_EQUAL(sg->nodes, NB_NODES);
	CU_ASSERT_EQUAL(sg->edges, nbEdges);

	tGraph_destroy(sg);

}

void tGraph_explore_test(void)
{
	int i, flag;
	// Graphe acyclic connexe
	int nodes_1[] =  {1,2,3};
	const int nb_nodes_1 = 3;
	// Graphe cyclic connexe
	int nodes_2[] = {1,3,4};
	const int nb_nodes_2 = 3;
	// Graphe non connexe
	int nodes_3[] = {1,2,3,8};
	const int nb_nodes_3 = 4;

	// cohérence de l'exploration ?

	// Graph for nodes_1
	tGraph* sg = tGraph_reduce(g, nodes_1, nb_nodes_1);
	int* explore = tGraph_explore(sg, 1);
	for(i = 0; i<sg->nodes; i++)
	{
		CU_ASSERT(explore[i] == 1);
	}
	// Graph for nodes_2
	sg = tGraph_reduce(g, nodes_2, nb_nodes_2);
	explore = tGraph_explore(sg, 1);
	flag = 0;
	for(i = 0; i<sg->nodes; i++)
	{
		CU_ASSERT(explore[i] >= 1);
		if(explore[i] > 1) flag = 1;
	}
	CU_ASSERT_EQUAL(flag, 1);
	// Graph for nodes_3
	sg = tGraph_reduce(g, nodes_3, nb_nodes_3);
	explore = tGraph_explore(sg, 1);
	flag = 0;
	for(i = 0; i<sg->nodes; i++)
	{
		if(explore[i] == 0) flag++;
	}
	CU_ASSERT_EQUAL(flag, 1);
	// free memory
	tGraph_destroy(sg);
	free(explore);
}

void tGraph_isTree_test(void)
{
	int nodes_1[] = {1,2,3};
	tGraph* sg = tGraph_reduce(g, nodes_1, 3);
	char val = tGraph_isTree(sg);
	CU_ASSERT_EQUAL(val, 1);

	int nodes_2[] = {1,3,4};
	sg = tGraph_reduce(g, nodes_2, 3);
	val = tGraph_isTree(sg);
	CU_ASSERT_EQUAL(val, 0);

	int nodes_3[] = {1,2,3,8};
	sg = tGraph_reduce(g, nodes_3, 4);
	val = tGraph_isTree(sg);
	CU_ASSERT_EQUAL(val, 0);
	// free memory
	tGraph_destroy(sg);
}


void tGraph_mst_test(void)
{
	int nodes[] = {1,2,3,4,6,7};
	int mstSize;
	tGraph* sg = tGraph_reduce(g, nodes, 7);
	tEdge* mst = tGraph_mst(sg, &mstSize);
}

int main(int argc, char** argv)
{
	CU_pSuite pSuite = NULL;

	// init the registry
	if(CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	pSuite = CU_add_suite("Graph_test_suite", init_suite, clean_suite);
	if(pSuite == NULL)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if((CU_add_test(pSuite,"tGraph_reduce",tGraph_reduce_test) == NULL))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if((CU_add_test(pSuite,"tGraph_explore",tGraph_explore_test) == NULL))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if((CU_add_test(pSuite,"tGraph_isTree",tGraph_isTree_test) == NULL))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");

	CU_cleanup_registry();

	exit(CU_get_error());

	return 0;
}

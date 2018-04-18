#include "uf.h"
#include "utils.h"
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define SIZE 10

UF* uf;

int init_suite(void)
{
	uf = UF_create(SIZE);
	return 0;
}

int clean_suite(void)
{
	UF_destroy(uf);
	return 0;
}

void uf_simpleTest(void)
{
	UF* luf = UF_clone(uf);

	int i;
	for(i = 3; i <= SIZE; i++)
	{
		UF_union(luf, i-2, i);
	}

	for(i = 1; i <= SIZE; i++)
	{
		if(i % 2 == 0)
		{
			CU_ASSERT_EQUAL(2, UF_find(luf, i));
		}
		else
		{
			CU_ASSERT_EQUAL(1, UF_find(luf, i));
		}
	}
}

void uf_cloneTest(void)
{
	int i;
	for(i = 1; i <= SIZE; i++)
	{
		CU_ASSERT_EQUAL(i , UF_find(uf, i));
	}
}

void uf_cpsTest(void)
{
	UF* luf = UF_clone(uf);
	int i, j;
	for(i = 3; i <= SIZE; i++)
	{
		UF_cunion(luf, i-2, i);
	}

	for(i = 1; i <= SIZE; i++)
	{
		if(i % 2 == 0)
		{
			CU_ASSERT_EQUAL(2, UF_find(luf, i));
		}
		else
		{
			CU_ASSERT_EQUAL(1, UF_find(luf, i));
		}
	}

	UF_buildClasses(luf);
	CU_ASSERT_EQUAL(2 , luf->nbClass);
	for(i = 0; i < luf->nbClass; i++)
	{
		for(j = 0; j < luf->cls[i].size; j++)
		{
			CU_ASSERT_EQUAL(luf->cls[i].elements[0], UF_find(luf, luf->cls[i].elements[j]));
		}
	}
}


int main(int argc, char** argv)
{

	CU_pSuite pSuite = NULL;

	// init the registry
	if(CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	pSuite = CU_add_suite("Union-Find tests", init_suite, clean_suite);
	if(pSuite == NULL)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if((CU_add_test(pSuite,"Simple Test",uf_simpleTest) == NULL))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if((CU_add_test(pSuite,"Clone test",uf_cloneTest) == NULL))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if((CU_add_test(pSuite,"CPS test",uf_cpsTest) == NULL))
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

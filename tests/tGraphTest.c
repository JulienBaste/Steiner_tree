#include "tGraph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int nodes[] = {1,2,3,4,6,7};
	int sg_nodes[] = {1,2,3,7};
	int i, mstSize, a, b;
	tEdge e;
	tGraph* g = tGraph_from_file("res/home.gr");
	tEdge* mst = tGraph_mstOfNodes(g, nodes, 6, 7, &mstSize);
	i = 0;
	while(i < mstSize)
	{
		e = mst[i];
		printf("Edge(%d,%d) : %d \n",e.x,e.y,e.val);
		i++;
	}


	tGraph* sg = tGraph_reduce(g, sg_nodes, 4);
	For_Graph_Edge(sg,a,b)
	{
		printf("sg edge : (%d,%d) \n",a,b);
	}
	free(mst);
	tGraph_destroy(g);
	return 0;
}

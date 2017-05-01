#include "b-tree.h"
using namespace std;

int main()
{
	//hello
	b_tree_class tree;
	for (int i = 0; i < 10; i++)
	{
		tree.btree_insert(i, i*i);
	}
	tree.show();
	return 0;
}
#pragma once
#include "b-tree.h"
#define T 2
using namespace std;

class b_tree_node
{
	private:
		bool leaf;
		int nkeys;
		int *key;
		int *value;
		b_tree_node **child;
		friend class b_tree_class;
	public:
		b_tree_node();
};

typedef b_tree_node *pointer_btree;
typedef b_tree_node **p_pointer_btree;

b_tree_node::b_tree_node()
{
	leaf = true;
	nkeys = 0;
	key = (int*)malloc(sizeof(*key) * 2 * T - 1);
	value = (int*)malloc(sizeof(*value) * 2 * T - 1);
	child = new b_tree_node*;
}


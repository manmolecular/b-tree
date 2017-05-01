#pragma once
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include "node-b-tree.h"
using namespace std;

class b_tree_class
{
private:
	pointer_btree root;
public:
	b_tree_class();
	pointer_btree btree_insert_nonfull(pointer_btree node, int key, int value);
	void btree_split_node(pointer_btree node, pointer_btree parent, int index);
	void btree_insert(int key, int value);
	void show();
	bool search(int key, int *value);
};

inline bool b_tree_class::search(int key, int *value)
{
	if (root != NULL)
	{
		return root->search(key, value);
	}
	return false;
}

inline b_tree_class::b_tree_class()
{
	root = NULL;
}

inline void b_tree_class::show()
{
	if (root != NULL)
	{
		root->show();
	}
	cout << endl;
}

pointer_btree b_tree_class::btree_insert_nonfull(pointer_btree node, int key, int value)
{
	int i = node->nkeys - 1;
	if (node->leaf)
	{
		while (i >= 0 && node->key[i] > key)
		{
			node->key[i + 1] = node->key[i];
			node->value[i + 1] = node->value[i];
			i--;
		}
		node->key[i + 1] = key;
		node->value[i + 1] = value;
		node->nkeys++;
	}
	else
	{
		while (i >= 0 && node->key[i] > key)
		{
			i--;
		}
		i++;
		if (node->child[i]->nkeys == 2 * T - 1)
		{
			btree_split_node(node->child[i], node, i);
			if (key > node->key[i])
			{
				i++;
			}
		}
		node = btree_insert_nonfull(node->child[i], key, value);
	}
	return node;
}

void b_tree_class::btree_insert(int key, int value)
{
	if (root == NULL)
	{
		root = new b_tree_node;
		root->nkeys = 1;
		root->leaf = true;
		root->key[0] = key;
		root->value[0] = value;
	}
	else if (root->nkeys == 2 * T - 1)
	{
		pointer_btree new_root = new b_tree_node;
		new_root->leaf = false;
		new_root->nkeys = 0;
		new_root->child[0] = root;
		btree_split_node(root, new_root, 0);
		int i = 0;
		if (new_root->key[0] < key)
		{
			i++;
		}
		btree_insert_nonfull(new_root->child[i], key, value);
		root = new_root;
	}
	else
	{
		btree_insert_nonfull(root, key, value);
	}
}

void b_tree_class::btree_split_node(pointer_btree node, pointer_btree parent, int index)
{
	pointer_btree temp = new b_tree_node;
	int i;

	temp->leaf = node->leaf;
	temp->nkeys = T - 1;
	for (i = 0; i < T - 1; i++)
	{
		temp->key[i] = node->key[T + i];
		temp->value[i] = node->value[T + i];
	}
	if (!node->leaf)
	{
		for (i = 0; i < T; i++)
		{
			temp->child[i] = node->child[i + T];
		}
	}
	node->nkeys = T - 1;

	/* Insert median key into parent node*/
	//for (i = parent->nkeys; i >= 0 && i <= index + 1; i--)			//не >=, а строго > - иначе child[-1] pos
	for (i = parent->nkeys; i >= index + 1; i--)
	{
		parent->child[i + 1] = parent->child[i];
	}
	parent->child[index + 1] = temp;

	//for (i = parent->nkeys - 1; i >= 0 && i <= index; i--)
	for (i = parent->nkeys - 1; i >= index; i--)
	{
		parent->key[i + 1] = parent->key[i];
		parent->value[i + 1] = parent->value[i];
	}
	parent->key[index] = node->key[T - 1];
	parent->value[index] = node->value[T - 1];
	parent->nkeys++;
}

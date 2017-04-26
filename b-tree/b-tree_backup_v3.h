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
		void btree_lookup(pointer_btree tree, int key, p_pointer_btree node, int *index);
		void btree_split_node(pointer_btree node, pointer_btree parent, int index);
		pointer_btree btree_insert_nonfull(pointer_btree node, int key, int value);
		pointer_btree btree_insert(int key, int value);
		void show_root();
		void show_root_recursive(pointer_btree _root);
};

inline void b_tree_class::show_root_recursive(pointer_btree _root)
{
	cout << "Output: " << endl;
	cout << "nkeys = " << _root->nkeys << endl << endl;
	for (int i = 0; i < _root->nkeys; i++)
	{
		cout << "INSERT #" << i << ": " << endl;
		cout << "leaf = " << std::boolalpha << _root->leaf << endl;
		cout << "key = " << _root->key[i] << endl;
		cout << "value = " << _root->value[i] << endl << endl;
	}
	if (!(_root->leaf))
	{
		for (int i = 0; i < _root->nkeys; i++)
		{ 
				show_root_recursive(_root->child[i]);
		}
	}
}

inline void b_tree_class::show_root()
{
	show_root_recursive(root);
}

inline b_tree_class::b_tree_class()
{
	root = NULL;
}

inline void b_tree_class::btree_lookup(pointer_btree tree, int key, p_pointer_btree node, int * index)
{
	int i;
	for (i = 0; i < tree->nkeys && key>tree->key[i]; )
	{
		i++;
	}
	if (i < tree->nkeys && key == tree->key[i])
	{
		*node = tree;
		*index = i;
		return;
	}
	if (!tree->leaf)
	{
		/* Disk read tree->child[i] */
		btree_lookup(tree, key, node, index);
	}
	else
	{
		*node = NULL;
	}
}

inline void b_tree_class::btree_split_node(pointer_btree node, pointer_btree parent, int index)
{
	pointer_btree temp = new b_tree_node;
	int i;

	temp->leaf = node->leaf;
	//cout << "temp->leaf: " << temp->leaf << endl;
	temp->nkeys = T - 1;
	//cout << "temp->nkeys: " << temp->nkeys << endl;
	for (i = 0; i < T - 1; i++)
	{
		temp->key[i] = node->key[T + i];
		//cout << "temp->key[i]: " << temp->key[i] << endl;
		temp->value[i] = node->value[T + i];
		//cout << "temp->value[i]: " << temp->value[i] << endl << endl;
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
	parent->child[index + 1] = temp;			// !!!!!!!


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

inline pointer_btree b_tree_class::btree_insert_nonfull(pointer_btree node, int key, int value)
{
	int i;
	i = node->nkeys - 1;
	if (node->leaf)
	{
		while (i > 0 && node->key[i] > key)
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
		while (i > 0 && node->key[i] > key)
		{
			i--;
		}
		//i++;

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

inline pointer_btree b_tree_class::btree_insert(int key, int value)
{
	pointer_btree new_root;
	if (root == NULL)
	{
		root = new b_tree_node;
		root->nkeys = 1;
		root->key[0] = key;
		root->value[0] = value;
		return root;
	}
	else if (root->nkeys == 2 * T - 1)
	{
		new_root = new b_tree_node;
		new_root->leaf = false;
		new_root->nkeys = 0;
		new_root->child[0] = root;
		btree_split_node(root, new_root, 0);
		root = new_root;
		return btree_insert_nonfull(new_root, key, value);
	}
	else
	{
		return btree_insert_nonfull(root, key, value);
	}
}


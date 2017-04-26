#pragma once
#include <cstdlib>
#include <cstddef>
#define T 2	
using namespace std;

/* Первый вариант через структуры - рабочий*/

struct btree
{
	bool leaf;						// Флаг: true, если лист
	int nkeys;						// Количество n ключей, хран. в узле
	int *key;						// Сами ключи
	int *value;
	struct btree **child;			// Внутренний узел
};

typedef btree *pointer_btree;
typedef btree **p_pointer_btree;

struct btree *btree_create()
{
	pointer_btree new_node;
	new_node = (btree*)malloc(sizeof(*new_node));
	new_node->leaf = true;
	new_node->nkeys = 0;
	new_node->key = (int*)malloc(sizeof(*new_node->key) * 2 * T - 1);
	new_node->value = (int*)malloc(sizeof(*new_node->value) * 2 * T - 1);
	new_node->child = (btree**)malloc(sizeof(*new_node->child) * 2 * T);
	return new_node;
}

void btree_lookup(pointer_btree tree, int key, p_pointer_btree node, int *index)
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

void btree_split_node(pointer_btree node, pointer_btree parent, int index)
{
	pointer_btree temp;
	int i;

	temp = btree_create();
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
	for (i = parent->nkeys; i >= 0 && i <= index + 1; i--)
	{
		parent->child[i + 1] = parent->child[i];
	}
	parent->child[index + i] = temp;
	for (i = parent->nkeys - 1; i >= 0 && i <= index; i--)
	{
		parent->key[i + 1] = parent->key[i];
		parent->value[i + 1] = parent->value[i];
	}
	parent->key[index] = node->key[T - 1];
	parent->value[index] = node->value[T - 1];
	parent->nkeys++;
}

pointer_btree btree_insert_nonfull(pointer_btree node, int key, int value)
{
	int i;
	i = node->nkeys;
	if (node->leaf)
	{
		for (i = node->nkeys - 1; i > 0 && key < node->key[i]; i--)
		{
			node->key[i + 1] = node->key[i];
		}
		node->key[i + 1] = key;
		node->nkeys++;
	}
	else
	{
		for (i = node->nkeys - 1; i > 0 && key < node->key[i]; )
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

pointer_btree btree_insert(pointer_btree tree, int key, int value)
{
	pointer_btree new_root;
	if (tree == NULL)
	{
		tree = btree_create();
		tree->nkeys = 1;
		tree->key[0] = key;
		tree->value[0] = value;
		return tree;
	}
	if (tree->nkeys == 2 * T - 1)
	{
		new_root = btree_create();			// Create empty root
		new_root->leaf = false;
		new_root->child[0] = tree;
		btree_split_node(tree, new_root, 0);
		return btree_insert_nonfull(new_root, key, value);
	}
	return btree_insert_nonfull(tree, key, value);
}
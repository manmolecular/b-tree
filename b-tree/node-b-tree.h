#pragma once
#include "b-tree.h"
#define T 3
using namespace std;

/* Класс узла b-дерева */

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
	~b_tree_node();
	void show();
	int search(int _key);
	int find_key(int _key);
};

typedef b_tree_node *pointer_btree;
typedef b_tree_node **p_pointer_btree;

/* Поиск ключа */
inline int b_tree_node::find_key(int _key)
{
	int i = 0;
	while (i < nkeys && key[i] < _key)
	{
		++i;
	}
	return i;
}

/* Поиск значения по ключу */
inline int b_tree_node::search(int _key)
{
	int i = find_key(_key);
	if (_key == key[i])
	{
		int _value = value[i];
		return _value;
	}
	if (leaf)
	{
		return false;
	}
	return child[i]->search(_key);
}

/* Конструктор */
inline b_tree_node::b_tree_node()
{
	leaf = true;
	nkeys = 0;
	//key = (int*)malloc(sizeof(*key) * 2 * T - 1);	
	//value = (int*)malloc(sizeof(*value) * 2 * T - 1);
	key = new int[2 * T - 1];
	value = new int[2 * T - 1];
	child = new b_tree_node*[2 * T];
}

/* Деструктор */
inline b_tree_node::~b_tree_node()
{
	free(key);
	free(value);
	free(child);
}

/* Отображение */
void b_tree_node::show()
{
	int i;
	for (i = 0; i < nkeys; i++)
	{
		if (!leaf)
		{
			cout << endl << "Child #" << i << ":";
			child[i]->show();
		}
		cout << endl << "(key: " << key[i] << ", value: " << value[i] << ")";
	}
	if (!leaf)
	{
		cout << endl << "Child #" << i << ":";
		child[i]->show();
	}
}
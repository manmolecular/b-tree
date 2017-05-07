#pragma once
#include "b-tree.h"
#define T 4
using namespace std;

/* ����� ���� b-������ */

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

/* ����� ����� */
inline int b_tree_node::find_key(int _key)
{
	int i = 0;
	while (i < nkeys && key[i] < _key)
	{
		++i;
	}
	return i;
}

/* ����� �������� �� ����� */
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

/* ����������� */
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

/* ���������� */
inline b_tree_node::~b_tree_node()
{
	free(key);
	free(value);
	free(child);
}

/* ����������� */
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
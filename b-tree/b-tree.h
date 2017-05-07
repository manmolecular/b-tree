#pragma once
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include "node-b-tree.h"
using namespace std;

/* ����� b-������ */

class b_tree_class
{
private:
	pointer_btree root;
public:
	b_tree_class();
	pointer_btree btree_insert_nonfull(pointer_btree node, int key, int value);
	void btree_split_node(pointer_btree node, pointer_btree parent, int index);
	void btree_insert(int key, int value);
	void btree_erase(int key, pointer_btree node);
	void btree_erase_helper(int key);
	void show();
	int search(int key);
	void remove_from_leaf(int i, pointer_btree node);
	void remove_from_nonleaf(int _i, pointer_btree node);
	int get_pred(int i, pointer_btree node);
	int get_succ(int i, pointer_btree node);
	void merge(int _i, pointer_btree node);
	void fill(int _i, pointer_btree node);
	void borrow_from_prev(int _i, pointer_btree node);
	void borrow_from_next(int _i, pointer_btree node);
};

inline void b_tree_class::borrow_from_next(int _i, pointer_btree node)
{
	/* -||-, ��� � _prev ������� */
	pointer_btree _child = node->child[_i];
	pointer_btree sibling = node->child[_i + 1];

	/* ���� �� _i ����� ����������� ��� ��������� ����
	� child[_i]*/
	_child->key[_child->nkeys] = node->key[_i];
	_child->value[_child->nkeys] = node->value[_i];

	/* ������ ������ ��� ����-����� ����������� ��� 
	��������� ������ � ������� child[_i]*/
	if (!(_child->leaf))
	{
		_child->child[_child->nkeys + 1] = sibling->child[0];
	}

	/* ������ ���� ����� ����������� � key[_i]*/
	node->key[_i] = sibling->key[0];
	node->value[_i] = sibling->value[0];

	/* �������� �� �� ��� ����� */
	for (int i = 1; i < sibling->nkeys; ++i)
	{
		sibling->key[i - 1] = sibling->key[i];
		sibling->value[i - 1] = sibling->value[i];
	}

	/* ... ������� ��������� */
	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->nkeys; ++i)
		{
			sibling->child[i - 1] = sibling->child[i];
		}
	}
	_child->nkeys += 1;
	sibling->nkeys -= 1;
	return;
}

/* ������� ��� ���������� ����� �� child[_i - 1] � �������
��� � child[_i]*/
inline void b_tree_class::borrow_from_prev(int _i, pointer_btree node)
{
	pointer_btree _child = node->child[_i];
	pointer_btree sibling = node->child[_i - 1];

	/* ����� ��������� ���� �� child[_i - 1] �����������
	� ������������ ���� � ���� �� [_i - 1] ����� 
	�� ������������� ���� ����������� ������ � 
	child[_i]. �������� ��� ����� � �������� �� 
	���� ��� ����� */

	for (int i = _child->nkeys - 1; i >= 0; --i)
	{
		_child->key[i + 1] = _child->key[i];
		_child->value[i + 1] = _child->value[i];
	}

	/* ���� child[_i] �� ����, �������� ��� ���������
	�� ���� ��� ����� */
	if (!_child->leaf)
	{
		for (int i = _child->nkeys; i >= 0; --i)
		{
			_child->child[i + 1] = _child->child[i];
		}
	}

	/* ������������� ������ ���� � child ������ _i-1 �����
	�� �������� ����*/
	_child->key[0] = node->key[_i - 1];

	/* ������ ���������� ������ �����
	������ ������� ������������� child-����*/
	if (!node->leaf)
	{
		_child->child[0] = sibling->child[sibling->nkeys];
	}

	/* ���������� ���� ����� � �������� */
	node->key[_i - 1] = sibling->key[sibling->nkeys - 1];
	_child->nkeys += 1;
	sibling->nkeys -= 1;
	return;
}

/* ������� ���������� child[_i], ������� ����� ������ ��� t-1 ������ */
inline void b_tree_class::fill(int _i, pointer_btree node)
{
	/* ���� ���������� child �������� ������, ��� T-1 ������, 
	�������� ���� �� ����� ����*/
	if (_i != 0 && node->child[_i - 1]->nkeys >= T)
	{
		borrow_from_prev(_i, node);
	}

	/* -||- ��� ���������� ���� */
	else if (_i != node->nkeys && node->child[_i + 1]->nkeys >= T)
	{
		borrow_from_next(_i, node);
	}

	/* ������� _i ������ � �����-"������"
	���� child �� _i ����� - ���������, ������� ��� � ���������� "������
	����� - � �����������*/
	else
	{
		if (_i != node->nkeys)
		{
			merge(_i, node);
		}
		else
		{
			merge(_i - 1, node);
		}
	}
	return;
}

/* ������� ������� _i � _i+1 child-��. _i + 1 ����� ������������� */
inline void b_tree_class::merge(int _i, pointer_btree node)
{
	pointer_btree _child = node->child[_i];
	pointer_btree sibling = node->child[_i + 1];

	/* ������� ���� �� �������� ���� � ��������� ��� �� 
	T - 1 ����� � ����� child[_i]*/
	_child->key[T - 1] = node->key[_i];
	_child->value[T - 1] = node->value[_i];

	/* �������� ����� �� _i+1 � _i child */
	for (int i = 0; i < sibling->nkeys; ++i)
	{
		_child->key[i + T] = sibling->key[i];
		_child->value[i + T] = sibling->value[i];
	}

	/* �������� ��������� �� ���������������� child-� */
	if (!_child->leaf)
	{
		for (int i = 0; i < sibling->nkeys; ++i)
		{
			_child->child[i + T] = sibling->child[i];
		}
	}

	/* ��������� ����������, ������������ ������� ������ 
	� �������� - �������� �� �� ��� ����� */
	for (int i = _i + 1; i < node->nkeys; ++i)
	{
		node->key[i - 1] = node->key[i];
		node->value[i - 1] = node->value[i];
	}

	/* �������� �����-��������� �� ��� ����� */
	for (int i = _i + 2; i <= node->nkeys; ++i)
	{
		node->child[i - 1] = node->child[i];
	}
	_child->nkeys += sibling->nkeys + 1;
	node->nkeys--;
	delete(sibling);
	return;
}

/* ������� ������ ������������� */
inline int b_tree_class::get_succ(int i, pointer_btree node)
{
	pointer_btree cur = node->child[i + 1];
	/* ��������� ����������� ����� �� ��� ���, 
	���� �� ����*/
	while (!cur->leaf)
	{
		cur = cur->child[0];
	}
	return cur->key[0];
}

/* ������� ������ ��������������� */
inline int b_tree_class::get_pred(int i, pointer_btree node)
{
	pointer_btree cur = node->child[i];
	/* ��������� ����������� ������ �� ��� ���, 
	���� �� ��������� ����� */
	while (!cur->leaf)
	{
		cur = cur->child[cur->nkeys];
	}

	/* ���������� ��������� ���� � �����*/
	return cur->key[cur->nkeys - 1];
}

/* �������� �� ��-����� */
void b_tree_class::remove_from_nonleaf(int _i, pointer_btree node)
{
	int k = node->key[_i];
	/*
	���� child �������� ����, ������� ������������ k, ��������
	��� ������� T ������, ������� ��������������� k � ���������
	� ������ � k. �������� k �� ����������. ��������� �������
	���������� � child[_i]
	*/
	if (node->child[_i]->nkeys >= T)
	{
		int pred = get_pred(_i, node);
		node->key[_i] = pred;
		btree_erase(pred, node->child[_i]);
	}
	/*
	���� �� child �� _i ����� �������� ������ T ������, 
	������� �� _i+1 child ����. ���� ���� ���� �������� ���
	������� � ������, ������� "����������" ��� k � ���������
	� ������ � _i + 1 child-e. �������� k �� succ. ����������
	������� succ � child[_i + 1]
	*/
	else if (node->child[_i + 1]->nkeys >= T)
	{
		int succ = get_succ(_i, node);
		node->key[_i] = succ;
		btree_erase(succ, node->child[_i + 1]);
	}
	/*
	���� �� child � _i � � _i + 1 ������� �������� ������,
	��� � ������, ������� k � ��, ��� � child[_i+1] � ����
	child[_i]. ������ � child[_i] ���������� 2� - 1 ������. 
	����������� child[_i+1] � ���������� ������� k �� 
	child[_i]*/
	else
	{
		merge(_i, node);
		btree_erase(k, node->child[_i]);
	}
	return;
}

/* �������� �� ����-����� */
void b_tree_class::remove_from_leaf(int _i, pointer_btree node)
{
	/* �������� ��� �������� �� ���� ������� ����� */
	for (int i = _i; i < node->nkeys - 1; i++)
	{
		node->key[i] = node->key[i + 1];
		node->value[i] = node->value[i + 1];
	}
	/* ��������� ���������� ������ */
	node->nkeys--;
}

/* �������� �� ������ �������� �� ������ */
inline void b_tree_class::btree_erase_helper(int key)
{
	if (root == NULL)
	{
		cout << "error";
		return;
	}
	pointer_btree node;
	node = root;
	btree_erase(key, node);
}

/* �������� �� b-������ */
void b_tree_class::btree_erase(int key, pointer_btree node)
{
	/* ������� ����, ������� ����� ������� ������ �� ���������*/
	int i = node->find_key(key);
	
	/* ���� ���� �� ��������� � ��������� � ���������*/
	if (i < node->nkeys && node->key[i] == key)
	{
		/* ���� ���� - ��� ����*/
		if (node->leaf)
		{
			/* �������� �� ����� */
			remove_from_leaf(i, node);
		}
		else
		{
			/* �������� �� ��-�����*/
			remove_from_nonleaf(i, node);
		}
	}
	else
	{
		/* ���� ���� ���� - ����, �� �������� ����� � �� �� ����������*/
		if (node->leaf)
		{
			cout << "The key " << key << "is does not exists in the tree" << endl;
			return;
		}
		
		/* ����, ������� ��������� �������, ��������� � ���������, 
		������ �������� �������� ������� ����*/
		bool flag = ((i == node->nkeys) ? true : false);

		/* ���� � ��������� ����, ��� ������ ���������� ����, 
		������� ������, ��� � ������, �� ��������� ���� ���� */
		if (node->child[i]->nkeys < T)
		{
			fill(i, node);
		}

		/* ���� ��������� �������� ���� ��� ����, �� ������ ���
		���� ���� ���������� ������ � ���������� �������� �����. 
		�����, �� ���������� �������� �� _i ����, ������� ������
		�������� ��� ������� � ������ */
		if (flag && i > node->nkeys)
		{
			btree_erase(key, node->child[i - 1]);
		}
		else
		{
			btree_erase(key, node->child[i]);
		}
	}
	return;
}

/* ����� �� ����� */
inline int b_tree_class::search(int key)
{
	if (root != NULL)
	{
		return root->search(key);
	}
	return false;
}

/* ����������� */
inline b_tree_class::b_tree_class()
{
	root = NULL;
}

/* ����� */
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
	//for (i = parent->nkeys; i >= 0 && i <= index + 1; i--)			//�� >=, � ������ > - ����� child[-1] pos
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

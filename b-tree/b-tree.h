#pragma once
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include "node-b-tree.h"
using namespace std;

/* Класс b-дерева */

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
	int get_pred(int i, pointer_btree node, int *_temp);
	int get_succ(int i, pointer_btree node, int *_temp);
	void merge(int _i, pointer_btree node);
	void fill(int _i, pointer_btree node);
	void borrow_from_prev(int _i, pointer_btree node);
	void borrow_from_next(int _i, pointer_btree node);
};

inline void b_tree_class::borrow_from_next(int _i, pointer_btree node)
{
	/* -||-, что и _prev функция */
	pointer_btree _child = node->child[_i];
	pointer_btree sibling = node->child[_i + 1];

	/* Ключ на _i месте вставляется как последний ключ
	в child[_i]*/
	_child->key[_child->nkeys] = node->key[_i];
	_child->value[_child->nkeys] = node->value[_i];

	/* Первый ребёнок для узла-брата вставляется как 
	последний ребёнок в текущий child[_i]*/
	if (!(_child->leaf))
	{
		_child->child[_child->nkeys + 1] = sibling->child[0];
	}

	/* Первый ключ брата вставляется в key[_i]*/
	node->key[_i] = sibling->key[0];
	node->value[_i] = sibling->value[0];

	/* Сдвигаем всё на шаг назад */
	for (int i = 1; i < sibling->nkeys; ++i)
	{
		sibling->key[i - 1] = sibling->key[i];
		sibling->value[i - 1] = sibling->value[i];
	}

	/* ... Включая указатели */
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

/* Функция для забираения ключа из child[_i - 1] и вставки
его в child[_i]*/
inline void b_tree_class::borrow_from_prev(int _i, pointer_btree node)
{
	pointer_btree _child = node->child[_i];
	pointer_btree sibling = node->child[_i - 1];

	/* Самый последний ключ из child[_i - 1] поднимается
	в родительский узел и ключ на [_i - 1] месте 
	из родительского узла вставляется первым в 
	child[_i]. Сдвигаем все ключи и значения на 
	один шаг вперёд */

	for (int i = _child->nkeys - 1; i >= 0; --i)
	{
		_child->key[i + 1] = _child->key[i];
		_child->value[i + 1] = _child->value[i];
	}

	/* Если child[_i] не лист, сдвигаем все указатели
	на один шаг вперёд */
	if (!_child->leaf)
	{
		for (int i = _child->nkeys; i >= 0; --i)
		{
			_child->child[i + 1] = _child->child[i];
		}
	}

	/* Устанавливаем первый ключ в child равный _i-1 ключу
	из текущего узла*/
	_child->key[0] = node->key[_i - 1];

	/* Делаем последнего ребёнка брата
	первым ребёнком оригинального child-узла*/
	if (!node->leaf)
	{
		_child->child[0] = sibling->child[sibling->nkeys];
	}

	/* Перемещаем ключ брата в родителя */
	node->key[_i - 1] = sibling->key[sibling->nkeys - 1];
	_child->nkeys += 1;
	sibling->nkeys -= 1;
	return;
}

/* Функция заполнения child[_i], который имеет меньше чем t-1 ключей */
inline void b_tree_class::fill(int _i, pointer_btree node)
{
	/* Если предыдущий child содержит больше, чем T-1 ключей, 
	забираем ключ от этого узла*/
	if (_i != 0 && node->child[_i - 1]->nkeys >= T)
	{
		borrow_from_prev(_i, node);
	}

	/* -||- для следующего узла */
	else if (_i != node->nkeys && node->child[_i + 1]->nkeys >= T)
	{
		borrow_from_next(_i, node);
	}

	/* Сливаем _i чайлда с узлом-"братом"
	если child на _i месте - последний, сливаем его с предыдущим "братом
	Иначе - с последующим*/
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

/* Функция слияния _i и _i+1 child-ов. _i + 1 затем освобождается */
inline void b_tree_class::merge(int _i, pointer_btree node)
{
	pointer_btree _child = node->child[_i];
	pointer_btree sibling = node->child[_i + 1];

	/* Достаем ключ из текущего узла и вставляем его на 
	T - 1 место в нашем child[_i]*/
	_child->key[T - 1] = node->key[_i];
	_child->value[T - 1] = node->value[_i];

	/* Копируем ключи из _i+1 в _i child */
	for (int i = 0; i < sibling->nkeys; ++i)
	{
		_child->key[i + T] = sibling->key[i];
		_child->value[i + T] = sibling->value[i];
	}

	/* Копируем указатели на соответствующиее child-ы */
	if (!_child->leaf)
	{
		for (int i = 0; i < sibling->nkeys; ++i)
		{
			_child->child[i + T] = sibling->child[i];
		}
	}

	/* Заполняем промежуток, образованный сдвигом ключей 
	и значений - сдвигаем всё на шаг назад */
	for (int i = _i + 1; i < node->nkeys; ++i)
	{
		node->key[i - 1] = node->key[i];
		node->value[i - 1] = node->value[i];
	}

	/* Сдвигаем чайлд-указатели на шаг назад */
	for (int i = _i + 2; i <= node->nkeys; ++i)
	{
		node->child[i - 1] = node->child[i];
	}
	_child->nkeys += sibling->nkeys + 1;
	node->nkeys--;
	delete(sibling);
	return;
}

/* Находим индекс последователя */
inline int b_tree_class::get_succ(int i, pointer_btree node, int *_temp)
{
	pointer_btree cur = node->child[i + 1];
	/* Двигаемся максимально влево до тех пор, 
	пока не лист*/
	while (!cur->leaf)
	{
		cur = cur->child[0];
	}
	*_temp = cur->value[0];
	return cur->key[0];
}

/* Находим индекс предшественника */
inline int b_tree_class::get_pred(int i, pointer_btree node, int *_temp)
{
	pointer_btree cur = node->child[i];
	/* Двигаемся максимально вправо до тех пор, 
	пока не достигнем листа */
	while (!cur->leaf)
	{
		cur = cur->child[cur->nkeys];
	}

	/* Возвращаем последний ключ и значение в листе*/
	*_temp = cur->value[cur->nkeys - 1];
	return cur->key[cur->nkeys - 1];
}

/* Удаление из не-листа */
void b_tree_class::remove_from_nonleaf(int _i, pointer_btree node)
{
	int k = node->key[_i];
	/*
	Если child текущего узла, который предшествует k, содержит
	как минимум T ключей, находим предшественника k в поддереве
	с корнем в k. Заменяем k на предыдущий. Рекурсиво удаляем
	предыдущий в child[_i]
	*/
	if (node->child[_i]->nkeys >= T)
	{
		int _temp = 0;
		int pred = get_pred(_i, node, &_temp);

		node->key[_i] = pred;
		node->value[_i] = _temp;

		btree_erase(pred, node->child[_i]);
	}
	/*
	Если же child на _i месте содержит меньше T ключей, 
	смотрим на _i+1 child узел. Если этот узел содержит как
	минимум Т ключей, находим "наследника" для k в поддереве
	с корнем в _i + 1 child-e. Заменяем k на succ. Рекурсивно
	удаляем succ в child[_i + 1]
	*/
	else if (node->child[_i + 1]->nkeys >= T)
	{
		int _temp = 0;
		int succ = get_succ(_i, node, &_temp);

		node->key[_i] = succ;
		node->value[_i] = _temp;

		btree_erase(succ, node->child[_i + 1]);
	}
	/*
	Если же child в _i и в _i + 1 позиции содержит меньше,
	чем Т ключей, сливаем k и всё, что в child[_i+1] в один
	child[_i]. Теперь в child[_i] содержится 2Т - 1 ключей. 
	Освобождаем child[_i+1] и рекурсивно удаляем k из 
	child[_i]*/
	else
	{
		merge(_i, node);
		btree_erase(k, node->child[_i]);
	}
	return;
}

/* Удаление из узла-листа */
void b_tree_class::remove_from_leaf(int _i, pointer_btree node)
{
	/* Сдвигаем все элементы на одну позицию влево */
	for (int i = _i; i < node->nkeys - 1; i++)
	{
		node->key[i] = node->key[i + 1];
		node->value[i] = node->value[i + 1];
	}
	/* Уменьшаем количество ключей */
	node->nkeys--;
}

/* Помощник по вызову удаления из дерева */
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

/* Удаление из b-дерева */
void b_tree_class::btree_erase(int key, pointer_btree node)
{
	/* Находим ключ, который нужно удалить вместе со значением*/
	int i = node->find_key(key);
	
	/* Если ключ не последний и совпадает с удаляемым*/
	if (i < node->nkeys && node->key[i] == key)
	{
		/* Если узел - это лист*/
		if (node->leaf)
		{
			/* Удаление из листа */
			remove_from_leaf(i, node);
		}
		else
		{
			/* Удаление из не-листа*/
			remove_from_nonleaf(i, node);
		}
	}
	else
	{
		/* Если этот узел - лист, то искомого ключа в нём не содержится*/
		if (node->leaf)
		{
			cout << "The key " << key << "is does not exists in the tree" << endl;
			return;
		}
		
		/* Ключ, который требуется удалить, находится в поддереве, 
		корнем которого является текущий узел*/
		bool flag = ((i == node->nkeys) ? true : false);

		/* Если у дочернего узла, где должен находиться ключ, 
		имеется меньше, чем Т ключей, мы заполняем этот узел */

		/******************/
		/* Где-то тут баг */
		/******************/

		if (node->child[i]->nkeys < T)
		{
			fill(i, node);
		}

		/* Если последний дочерний узел был слит, он должен был
		быть слит рекурсивно вместе с предыдущим дочерним узлом. 
		Иначе, мы рекурсивно проходим по _i узлу, который теперь
		содержит как минимум Т ключей */
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

/* Поиск по ключу */
inline int b_tree_class::search(int key)
{
	if (root != NULL)
	{
		return root->search(key);
	}
	return false;
}

/* Конструктор */
inline b_tree_class::b_tree_class()
{
	root = NULL;
}

/* Вывод */
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

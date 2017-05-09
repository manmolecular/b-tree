#include "b-tree.h"
#include <ctime>
#include <string>
#define _insert_count 10
#define _delete_count _insert_count - 2
using namespace std;

/* Поиск */
int search(b_tree_class tree)
{
	cout << endl << "Search request (key): ";
	int s_req;
	cin >> s_req;
	int value = tree.search(s_req);
	if (value)
	{
		cout << "Value is: " << value << endl;
		return value;
	}
	else
	{
		cout << "Not found" << endl;
	}
}

/* Удаление */
void erase(b_tree_class tree)
{
	int e_req = 0;
	tree.show();
	while (e_req != 999)
	{
		cout << endl << "Erase request (key): ";
		cin >> e_req;
		tree.btree_erase_helper(e_req);
		tree.show();
	}
}

/* Удаление с начала*/
void erase_all_from_begin(b_tree_class tree)
{
	tree.show();
	for (int i = 0; i < _delete_count; i++)
	{
		tree.btree_erase_helper(i);
		tree.show();
	}
}

/* Удаление с конца */
void erase_all_from_end(b_tree_class tree)
{
	tree.show();
	for (int i = 0; i < _delete_count; i++)
	{
		tree.btree_erase_helper(_delete_count + 1 - i);
		tree.show();
	}
}

int main()
{
	b_tree_class tree;

	srand(time(0));
	for (int i = 0; i < _insert_count; i++)
	{
		tree.btree_insert(i, i*rand() % 100);
	}
	erase(tree);
	//erase_all_from_begin(tree);
	//erase_all_from_end(tree);
	return 0;
}
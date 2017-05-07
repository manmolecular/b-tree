#include "b-tree.h"
#include <ctime>
#include <string>
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
	cout << endl << "Erase request (key): ";
	int e_req;
	cin >> e_req;
	tree.btree_erase_helper(e_req);
}

int main()
{
	b_tree_class tree;
	srand(time(0));
	for (int i = 0; i < 101; i++)
	{
		tree.btree_insert(i, i*rand() % 100);
	}
	tree.show();
	search(tree);
	erase(tree);
	tree.show();
	return 0;
}
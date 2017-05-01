#include "b-tree.h"
#include <ctime>
#include <string>
using namespace std;

//Комментарии на русском в UTF-8

int main()
{
	b_tree_class tree;
	srand(time(0));
	for (int i = 0; i < 30; i++)
	{
		tree.btree_insert(i, i*rand() % 100);
	}
	tree.show();

	cout << endl << "Search request (key): ";
	int s_req;
	int value;
	cin >> s_req;
	if (tree.search(s_req, &value))
	{
		cout << "Value is: " << value << endl;
	}
	else
	{
		cout << "Not found" << endl;
	}
	return 0;
}
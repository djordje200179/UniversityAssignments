#include <iostream>
#include "BStarTree.hpp"
#include <fstream>
#include <queue>

using namespace std;

int main() {
	BStarTree tree(4);

	ifstream file("sample.txt");
	file >> tree;

	//tree -= "sir";
	//tree -= "sljiva";

	cout << tree << endl;

	return 0;
}
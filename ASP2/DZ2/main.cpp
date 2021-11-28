#include <iostream>
#include "BStarTree.hpp"
#include <fstream>
#include <queue>

using namespace std;

void testPointers(const BStarTree& tree) {
	queue<BStarTree::Node*> nodes;
	nodes.push(tree.root);

	while (!nodes.empty()) {
		auto curr = nodes.front();
		nodes.pop();

		for (auto child : curr->children) {
			if (!child)
				continue;

			nodes.push(child);

			if (child->parent != curr) {
				cout << "Parent mismatch on node \"" << child->keys[0] << '"' << endl;
				cout << "\treal parent is \"" << curr->keys[0] << '"' << endl;
				cout << "\twrong parent is \"" << (child->parent ? child->parent->keys[0] : "NULL") << '"' << endl;
				cout << endl;
			}
		}
	}

	cout << endl;
}

int main() {
	BStarTree tree(4);

	ifstream file("sample.txt");
	file >> tree;

	cout << tree << endl;

	for(int i = 0; i < 15; i++)
		cout << tree.findKthKey(i) << endl;

	return 0;
}
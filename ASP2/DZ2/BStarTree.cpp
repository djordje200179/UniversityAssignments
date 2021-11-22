#include "BStarTree.hpp"
#include <cmath>
#include <stack>

using namespace std;

BStarTree::BStarTree(int degree) : degree(degree), maxKeys(degree - 1),
								   minNodeKeys(ceil((2 * degree - 1) / 3.0 - 1)), 
								   maxRootKeys(2 * floor((2 * degree - 2) / 3.0)) {}

BStarTree::~BStarTree() {
	stack<Node*> traversalStack, postorderStack;

	traversalStack.push(root);
	while (!traversalStack.empty()) {
		auto node = traversalStack.top();
		traversalStack.pop();

		if (!node)
			continue;

		postorderStack.push(node);

		for (auto child : node->children)
			traversalStack.push(child);
	}

	while (!postorderStack.empty()) {
		auto node = postorderStack.top();
		postorderStack.pop();

		delete node;
	}
}
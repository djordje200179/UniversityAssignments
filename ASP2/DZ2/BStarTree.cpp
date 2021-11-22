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

bool BStarTree::keyExists(CStr key) const {
	return findKey(key).node != nullptr;
}

BStarTree::Position BStarTree::findKey(CStr key) const {
	auto curr = root;

	while (curr) {
		for (size_t i = 0; i < curr->keys.size(); i++)
			if (curr->keys[i] == key)
				return { curr, i };
			else if (curr->keys[i] > key) {
				curr = curr->children[i];
				break;
			}

		curr = curr->children.back();
	}

	return { nullptr, 0 };
}

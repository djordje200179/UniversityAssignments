#include "Tree.hpp"
#include "Root.hpp"
#include <cmath>
#include <stack>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <iostream>

using namespace std;

BStarTree::Tree::~Tree() {
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

bool BStarTree::Tree::keyExists(CStr key) const {
	return findKey(key).node != nullptr;
}

bool BStarTree::Tree::addKey(CStr key) {
	if (!root)
		root = new Root();

	auto curr = root;
	while (!curr->isLeaf()) {
		int i;
		for (i = 0; i < curr->keyCount(); i++)
			if (curr->keys[i] == key)
				return false;
			else if (curr->keys[i] > key)
				break;

		curr = curr->children[i];
	}

	curr->addKey(key);

	while (true) {
		auto allowedKeys = curr == root ? MAX_ROOT_KEYS : MAX_NODE_KEYS;
		if (curr->keyCount() <= allowedKeys)
			break;

		auto spilled = curr->trySpill(SpillType::OVER);
		if (spilled)
			break;
		else {
			curr->split();
			if (curr == root)
				break;
			else
				curr = curr->parent;
		}
	}

	return true;
}

bool BStarTree::Tree::removeKey(CStr key) {
	auto position = findKey(key);

	if (!position)
		return false;

	if (!position.node->isLeaf()) {
		auto successor = findSuccessor(position);
		swap(position.getKey(), successor.getKey());

		position = successor;
	}

	auto curr = position.node;
	curr->removeKey(position.index);

	while (true) {
		auto allowedKeys = curr == root ? 1 : MIN_NODE_KEYS;
		if (curr->keyCount() >= allowedKeys)
			break;

		auto spilled = curr->trySpill(SpillType::UNDER);
		if (spilled)
			break;
		else {
			auto right = curr->getRight(), left = curr->getLeft();

			curr->split();
			(left && right ? curr : (right ? right : left))->join();
			if (curr == root)
				break;
			else
				curr = curr->parent;
		}
	}

	return true;
}

void BStarTree::Tree::printTree(ostream& os) const {
	stack<Node*> traversalStack;
	traversalStack.push(root);

	while (!traversalStack.empty()) {
		auto node = traversalStack.top();
		traversalStack.pop();

		if (node != root)
			node->level = node->parent->level + 1;

		node->print(os);

		for (auto it = node->children.rbegin(); it != node->children.rend(); it++)
			if (*it)
				traversalStack.push(*it);
	}
}

void BStarTree::Tree::inputWords(istream& is) {
	string line;
	while (getline(is, line))
		addKey(line);
}

BStarTree::CStr BStarTree::Tree::findKthKey(int k) const {
	Position curr = { root, 0 };
	while (!curr.node->isLeaf())
		curr = { curr.node->children.front(), 0 };
	
	for (int i = 0; i < k; i++)
		curr = findSuccessor(curr);

	return curr.getKey();
}

BStarTree::Position BStarTree::Tree::findKey(CStr key) const {
	auto curr = root;

	while (curr) {
		int i;
		for (i = 0; i < curr->keys.size(); i++)
			if (curr->keys[i] == key)
				return { curr, i };
			else if (curr->keys[i] > key)
				break;

		curr = curr->children[i];
	}

	return { nullptr, 0 };
}

BStarTree::Position BStarTree::Tree::findSuccessor(Position position) const {
	if (position.node->isLeaf()) {
		if (position.index != position.node->keys.size() - 1)
			return ++position;

		do {
			auto parentNode = position.node->parent;

			if (!parentNode)
				return { nullptr, 0 };

			position = { parentNode, position.node->getIndexInParent() };
		} while (position.index == position.node->children.size() - 1);
	} else {
		position++;
		while (!position.node->isLeaf())
			position = { position.node->children[position.index], 0 };
	}

	return position;
}
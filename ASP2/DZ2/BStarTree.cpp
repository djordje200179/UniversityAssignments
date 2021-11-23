#include "BStarTree.hpp"
#include <cmath>
#include <stack>

using namespace std;

bool BStarTree::Node::isLeaf() {
	return children.front() == nullptr;
}

bool BStarTree::Node::canAddKey(int maxKeys) {
	return isLeaf() && children.size() != maxKeys;
}

BStarTree::BStarTree(int degree) : DEGREE(degree), MAX_KEYS(degree - 1),
								   MAX_ROOT_KEYS(2 * floor((2 * degree - 2) / 3.0)),
								   MIN_NODE_KEYS(ceil((2 * degree - 1) / 3.0 - 1)) {}

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

BStarTree::CStr BStarTree::findKthKey(int k) const {
	stack<Position> s;
	Position curr = { root, 0 };

	int cnt = 0;
	while (curr || !s.empty()) {
		while (curr) {
			s.push(curr);
			curr = { curr.node->children[curr.index], 0 };
		}

		curr = s.top(), s.pop();

		if (cnt == k)
			return curr;

		cnt++;

		curr++;
	}
}

BStarTree::Position BStarTree::findKey(CStr key) const {
	auto curr = root;

	while (curr) {
		for (int i = 0; i < curr->keys.size(); i++)
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

BStarTree::Position BStarTree::findSuccessor(Position position) {
	if (position.node->isLeaf()) {
		while (position.index == position.node->children.size() - 1) {
			auto node = position.node;
			auto parentNode = node->positionInParent.node;

			if (!parentNode)
				return { nullptr, 0 };

			auto parentNodePosition = parentNode->positionInParent.index;

			position = { parentNode, parentNodePosition };
		}

		return ++position;
	} else {
		position++;
		while (!position.node->isLeaf())
			position = { position.node->children[position.index], 0 };

		return position;
	}
}
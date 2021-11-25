#include "BStarTree.hpp"
#include <cmath>
#include <stack>
#include <algorithm>

using namespace std;

BStarTree::Node::Node(Node* parent) : parent(parent) {
	children.push_back(nullptr);
}

BStarTree::Node::Node(Node* parent,
					  const std::vector<std::string>& initKeys,
					  const std::vector<Node*>& initChildren) : parent(parent), children(initChildren), keys(initKeys) {}

bool BStarTree::Node::isLeaf() {
	return children.front() == nullptr;
}

bool BStarTree::Node::canAddKey(int maxKeys) {
	return keys.size() != maxKeys;
}

static int sortedInsert(vector<string>& keys, const string& key) {
	int i;
	for (i = 0; i < keys.size() && keys[i] < key; i++);
	keys.insert(keys.begin() + i, key);

	return i;
}

void BStarTree::Node::addKey(CStr key) {
	sortedInsert(keys, key);
	children.push_back(nullptr);
}

int BStarTree::Node::getIndexInParent() {
	auto itFind = find(parent->children.begin(), parent->children.end(), this);
	auto itStart = parent->children.begin();

	return distance(itStart, itFind);
}

BStarTree::Node* BStarTree::Node::getLeft() {
	auto index = getIndexInParent() - 1;

	return index >= 0 ? parent->children[index] : nullptr;
}

BStarTree::Node* BStarTree::Node::getRight() {
	auto index = getIndexInParent() + 1;

	return index < parent->children.size() ? parent->children[index] : nullptr;
}

BStarTree::BStarTree(int degree)
	: DEGREE(degree), MAX_KEYS(degree - 1),
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

bool BStarTree::addKey(CStr key) {
	if (!root)
		root = new Node();

#pragma region Pronalazak lista
	Node* curr = root;
	while (!curr->isLeaf()) {
		int i;
		for (i = 0; i < curr->keys.size(); i++)
			if (curr->keys[i] == key)
				return false;
			else if (curr->keys[i] > key)
				break;

		curr = curr->children[i];
	}
#pragma endregion

#pragma region Dodavanje u list
	if (curr->canAddKey(curr == root ? MAX_ROOT_KEYS : MAX_KEYS)) {
		curr->addKey(key);

		return true;
	}
#pragma endregion

#pragma region Podjela korijena
	if (curr == root) {
		curr->addKey(key);

		int halfSize = curr->children.size() / 2;

		vector<string> keysLow(curr->keys.begin(), curr->keys.begin() + halfSize - 1);
		string keyMiddle = curr->keys[halfSize - 1];
		vector<string> keysHigh(curr->keys.begin() + halfSize, curr->keys.end());

		vector<Node*> childrenLow(curr->children.begin(), curr->children.begin() + halfSize);
		vector<Node*> childrenHigh(curr->children.begin() + halfSize, curr->children.end());

		Node* leftNode = new Node(curr, keysLow, childrenLow);
		Node* rightNode = new Node(curr, keysHigh, childrenHigh);

		root->keys.clear();
		root->keys.push_back(keyMiddle);

		root->children.clear();
		root->children.push_back(leftNode);
		root->children.push_back(rightNode);

		return true;
	}
#pragma endregion 

	auto right = curr->getRight(), left = curr->getLeft();
	Node* sibling;
#pragma region Presipanje u susjedni cvor
	sibling = nullptr;
	if (right && right->canAddKey(MAX_KEYS))
		sibling = right;
	else if (left && left->canAddKey(MAX_KEYS))
		sibling = left;

	if (sibling) {
		auto isRight = sibling == right;

		sibling->addKey(isRight ? curr->keys.back() : curr->keys.front());
		sibling->children.pop_back();

		if (isRight) {
			sibling->children.insert(sibling->children.begin(), curr->children.back());
			curr->children.pop_back();
		} else {
			sibling->children.push_back(curr->children.front());
			curr->children.erase(curr->children.begin());
		}

		curr->addKey(key);

		return true;
	}
#pragma endregion

#pragma region Podjela 2 u 3
	sibling = right ? right : left;

	auto parent = curr->parent;
	CStr divider = parent->keys[(sibling == right ? curr : left)->getIndexInParent()];

	vector<string> allKeys;
	vector<Node*> allChildren;
	if (sibling == right) {
		allKeys.insert(allKeys.end(), curr->keys.begin(), curr->keys.end());
		allKeys.insert(allKeys.end(), divider);
		allKeys.insert(allKeys.end(), sibling->keys.begin(), sibling->keys.end());

		allChildren.insert(allChildren.end(), curr->children.begin(), curr->children.end());
		allChildren.insert(allChildren.end(), sibling->children.begin(), sibling->children.end());
	} else {
		allKeys.insert(allKeys.end(), sibling->keys.begin(), sibling->keys.end());
		allKeys.insert(allKeys.end(), divider);
		allKeys.insert(allKeys.end(), curr->keys.begin(), curr->keys.end());

		allChildren.insert(allChildren.end(), sibling->children.begin(), sibling->children.end());
		allChildren.insert(allChildren.end(), curr->children.begin(), curr->children.end());
	}


	int index1 = (2 * DEGREE - 2) / 3;
	int index2 = index1 + 1 + (2 * DEGREE - 1) / 3;

	vector<string> leftKeys(allKeys.begin(), allKeys.begin() + index1);
	vector<string> middleKeys(allKeys.begin() + index1 + 1, allKeys.begin() + index2);
	vector<string> rightKeys(allKeys.begin() + index2 + 1, allKeys.end());

	vector<Node*> leftChildren(allChildren.begin(), allChildren.begin() + index1 + 1);
	vector<Node*> middleChildren(allChildren.begin() + index1 + 1, allChildren.begin() + index2 + 1);
	vector<Node*> rightChildren(allChildren.begin() + index2 + 1, allChildren.end());

	delete curr;
	delete sibling;

	auto leftNode = new Node(parent, leftKeys, leftChildren);
	auto middleNode = new Node(parent, middleKeys, middleChildren);
	auto rightNode = new Node(parent, rightKeys, rightChildren);

	auto positionInParent = distance(parent->keys.begin(), find(parent->keys.begin(), parent->keys.end(), divider));
	parent->keys[positionInParent] = allKeys[index2];
	parent->keys.insert(parent->keys.begin() + positionInParent, allKeys[index1]);
	parent->children[positionInParent + 1] = rightNode;
	parent->children[positionInParent] = middleNode;
	parent->children.insert(parent->children.begin() + positionInParent, leftNode);
#pragma endregion
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

BStarTree::Position BStarTree::findSuccessor(Position position) {
	if (position.node->isLeaf()) {
		while (position.index == position.node->children.size() - 1) {
			auto node = position.node;
			auto parentNode = node->parent;

			if (!parentNode)
				return { nullptr, 0 };

			auto parentNodePosition = parentNode->getIndexInParent();

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
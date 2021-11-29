#include "BStarTree.hpp"
#include <cmath>
#include <stack>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <iostream>

using namespace std;

BStarTree::Node::Node() : parent(nullptr), level(0) {
	children.push_back(nullptr);
}

BStarTree::Node::Node(Node* parent,
					  const vector<string>& initKeys,
					  const vector<Node*>& initChildren) : parent(parent), children(initChildren), keys(initKeys) {
	updateChildren();
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

void BStarTree::Node::removeKey(int index) {
	keys.erase(keys.begin() + index);
	children.erase(children.begin() + index + 1);
}

int BStarTree::Node::getIndexInParent() {
	auto itStart = parent->children.begin();
	auto itFind = find(itStart, parent->children.end(), this);
	auto index = distance(itStart, itFind);

	return index;
}

BStarTree::Node* BStarTree::Node::getLeft() {
	auto index = getIndexInParent() - 1;

	return index >= 0 ? parent->children[index] : nullptr;
}

BStarTree::Node* BStarTree::Node::getRight() {
	auto index = getIndexInParent() + 1;

	return index < parent->children.size() ? parent->children[index] : nullptr;
}

void BStarTree::Node::print(ostream& os) const {
	size_t boxSize = 10;

	for (int i = 0; i < level; i++)
		os << '\t';
	for (int i = 0; i < boxSize + 3; i++)
		os << '-';
	os << '\n';

	for (CStr key : keys) {
		for (int i = 0; i < level; i++)
			os << '\t';
		os << "| " << left << setw(boxSize) << key << '|' << '\n';
	}

	for (int i = 0; i < level; i++)
		os << '\t';
	for (int i = 0; i < boxSize + 3; i++)
		os << '-';
	os << '\n';
}

void BStarTree::Node::split() {
	auto rightSibling = getRight(), leftSibling = getLeft();
	auto sibling = rightSibling ? rightSibling : leftSibling;
	auto isRight = sibling == rightSibling;

	auto dividerIndex = getIndexInParent() - !isRight;
	CStr divider = parent->keys[dividerIndex];

	vector<string> allKeys;
	vector<Node*> allChildren;
	if (isRight) {
		allKeys.insert(allKeys.end(), keys.begin(), keys.end());
		allKeys.insert(allKeys.end(), divider);
		allKeys.insert(allKeys.end(), sibling->keys.begin(), sibling->keys.end());

		allChildren.insert(allChildren.end(), children.begin(), children.end());
		allChildren.insert(allChildren.end(), sibling->children.begin(), sibling->children.end());
	} else {
		allKeys.insert(allKeys.end(), sibling->keys.begin(), sibling->keys.end());
		allKeys.insert(allKeys.end(), divider);
		allKeys.insert(allKeys.end(), keys.begin(), keys.end());

		allChildren.insert(allChildren.end(), sibling->children.begin(), sibling->children.end());
		allChildren.insert(allChildren.end(), children.begin(), children.end());
	}

	int maxKeys = keys.size() - 1;
	int index1 = 2 * maxKeys / 3;
	int index2 = index1 + 1 + (2 * maxKeys + 1) / 3;

	vector<string> leftKeys(allKeys.begin(), allKeys.begin() + index1);
	vector<string> middleKeys(allKeys.begin() + index1 + 1, allKeys.begin() + index2);
	vector<string> rightKeys(allKeys.begin() + index2 + 1, allKeys.end());

	vector<Node*> leftChildren(allChildren.begin(), allChildren.begin() + index1 + 1);
	vector<Node*> middleChildren(allChildren.begin() + index1 + 1, allChildren.begin() + index2 + 1);
	vector<Node*> rightChildren(allChildren.begin() + index2 + 1, allChildren.end());

	keys = middleKeys;
	children = middleChildren;
	updateChildren();

	if (isRight) {
		sibling->keys = rightKeys;
		sibling->children = rightChildren;
		sibling->updateChildren();

		auto newNode = new Node(parent, leftKeys, leftChildren);

		parent->keys.insert(parent->keys.begin() + dividerIndex, allKeys[index1]);
		parent->keys[dividerIndex + 1] = allKeys[index2];

		parent->children.insert(parent->children.begin() + dividerIndex, newNode);

	} else {
		sibling->keys = leftKeys;
		sibling->children = leftChildren;

		auto newNode = new Node(parent, rightKeys, rightChildren);

		parent->keys[dividerIndex] = allKeys[index1];
		parent->keys.insert(parent->keys.begin() + dividerIndex + 1, allKeys[index2]);

		parent->children.insert(parent->children.begin() + dividerIndex + 2, newNode);
	}
}

void BStarTree::Node::join() {
	auto right = getRight(), left = getLeft();
	auto index1 = left->getIndexInParent();

	vector<string> allKeys;
	vector<Node*> allChildren;

	allKeys.insert(allKeys.end(), left->keys.begin(), left->keys.end());
	allKeys.insert(allKeys.end(), parent->keys[index1]);
	allKeys.insert(allKeys.end(), this->keys.begin(), this->keys.end());
	allKeys.insert(allKeys.end(), parent->keys[index1 + 1]);
	allKeys.insert(allKeys.end(), right->keys.begin(), right->keys.end());

	allChildren.insert(allChildren.end(), left->children.begin(), left->children.end());
	allChildren.insert(allChildren.end(), this->children.begin(), this->children.end());
	allChildren.insert(allChildren.end(), right->children.begin(), right->children.end());

	left->keys = vector<string>(allKeys.begin(), allKeys.begin() + allKeys.size() / 2);
	parent->keys[index1] = allKeys[allKeys.size() / 2];
	parent->keys.erase(parent->keys.begin() + index1 + 1);
	right->keys = vector<string>(allKeys.begin() + allKeys.size() / 2 + 1, allKeys.end());

	left->children = vector<Node*>(allChildren.begin(), allChildren.begin() + allChildren.size() / 2);
	parent->children.erase(parent->children.begin() + index1 + 1);
	right->children = vector<Node*>(allChildren.begin() + allChildren.size() / 2, allChildren.end());

	left->updateChildren();
	right->updateChildren();

	delete this;
}

void BStarTree::Node::spill(Node* from, Node* to) {
	auto onRight = from->getRight() == to;
	auto left = onRight ? from : to, right = onRight ? to : from;

	auto dividerIndex = left->getIndexInParent();
	auto& divider = left->parent->keys[dividerIndex];

	vector<string> allKeys;
	vector<Node*> allChildren;

	allKeys.insert(allKeys.end(), left->keys.begin(), left->keys.end());
	allKeys.insert(allKeys.end(), divider);
	allKeys.insert(allKeys.end(), right->keys.begin(), right->keys.end());

	allChildren.insert(allChildren.end(), left->children.begin(), left->children.end());
	allChildren.insert(allChildren.end(), right->children.begin(), right->children.end());

	left->keys = vector<string>(allKeys.begin(), allKeys.begin() + allKeys.size() / 2);
	divider = allKeys[allKeys.size() / 2];
	right->keys = vector<string>(allKeys.begin() + allKeys.size() / 2 + 1, allKeys.end());

	left->children = vector<Node*>(allChildren.begin(), allChildren.begin() + allChildren.size() / 2);
	right->children = vector<Node*>(allChildren.begin() + allChildren.size() / 2, allChildren.end());

	left->updateChildren();
	right->updateChildren();
}

bool BStarTree::Node::trySpill() {
	auto right = getRight(), left = getLeft();
	Node* sibling = nullptr;

	int maxKeys = keys.size() - 1;
	if (right && right->keyCount() < maxKeys)
		sibling = right;
	else if (left && left->keyCount() < maxKeys)
		sibling = left;

	if (!sibling)
		return false;

	spill(this, sibling);

	return true;
}

void BStarTree::Node::updateChildren() {
	for (auto child : children)
		if (child)
			child->parent = this;
}

void BStarTree::Root::split() {
	auto halfSize = keys.size() / 2;

	vector<string> keysLow(keys.begin(), keys.begin() + halfSize);
	string keyMiddle = keys[halfSize];
	vector<string> keysHigh(keys.begin() + halfSize + 1, keys.end());

	vector<Node*> childrenLow(children.begin(), children.begin() + halfSize + 1);
	vector<Node*> childrenHigh(children.begin() + halfSize + 1, children.end());

	children.clear();
	children.push_back(new Node(this, keysLow, childrenLow));
	children.push_back(new Node(this, keysHigh, childrenHigh));

	keys.clear();
	keys.push_back(keyMiddle);
}

void BStarTree::Root::join() {
	
}

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

		auto spilled = curr->trySpill();
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

bool BStarTree::removeKey(CStr key) {
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

		auto right = curr->getRight(), left = curr->getLeft();
		Node* sibling = nullptr;

		if (right && right->keyCount() >= MIN_NODE_KEYS + 1)
			sibling = right;
		else if (left && left->keyCount() >= MIN_NODE_KEYS + 1)
			sibling = left;

		if (sibling) {
			Node::spill(sibling, curr);
			break;
		}
		
		(left && right ? curr : (right ? right : left))->join();

		if (curr == root)
			break;
		else
			curr = curr->parent;
	}

	return true;
}

void BStarTree::printTree(ostream& os) const {
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

void BStarTree::inputWords(istream& is) {
	string line;
	while (getline(is, line))
		addKey(line);
}

BStarTree::CStr BStarTree::findKthKey(int k) const {
	Position curr = { root, 0 };
	while (!curr.node->isLeaf())
		curr = { curr.node->children.front(), 0 };
	
	for (int i = 0; i < k; i++)
		curr = findSuccessor(curr);

	return curr.getKey();
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

BStarTree::Position BStarTree::findSuccessor(Position position) const {
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
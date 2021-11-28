#include "BStarTree.hpp"
#include <cmath>
#include <stack>
#include <algorithm>
#include <utility>
#include <iomanip>

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

void BStarTree::Node::split(int maxKeys) {
	auto right = getRight(), left = getLeft();
	auto sibling = right ? right : left;
	auto isRight = sibling == right;

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

		auto newNode = new Node(parent, rightKeys, leftChildren);

		parent->keys[dividerIndex] = allKeys[index1];
		parent->keys.insert(parent->keys.begin() + dividerIndex + 1, allKeys[index2]);

		parent->children.insert(parent->children.begin() + dividerIndex + 2, newNode);
	}
}

void BStarTree::Node::join(int minKeys) {}

void BStarTree::Node::spillInto(Node* sibling) {
	auto right = getRight(), left = getLeft();
	auto isRight = sibling == right;

	auto dividerIndex = getIndexInParent() - !isRight;
	auto& divider = parent->keys[dividerIndex];

	auto movingKeyIterator = isRight ? keys.end() - 1 : keys.begin();
	auto movingChildIterator = isRight ? children.end() - 1 : children.begin();

	sibling->keys.insert(isRight ? sibling->keys.begin() : sibling->keys.end(), divider);
	divider = *movingKeyIterator;
	keys.erase(movingKeyIterator);

	auto movingChild = *movingChildIterator;
	if (movingChild)
		movingChild->parent = sibling;
	sibling->children.insert(isRight ? sibling->children.begin() : sibling->children.end(), movingChild);
	children.erase(movingChildIterator);
}

bool BStarTree::Node::spill(int maxKeys) {
	auto right = getRight(), left = getLeft();
	Node* sibling = nullptr;

	if (right && right->keyCount() < maxKeys)
		sibling = right;
	else if (left && left->keyCount() < maxKeys)
		sibling = left;

	if (!sibling)
		return false;

	spillInto(sibling);

	return true;
}

void BStarTree::Node::updateChildren() {
	for (auto child : children)
		if (child)
			child->parent = this;
}

void BStarTree::Root::split(int maxKeys) {
	int halfSize = ceil(maxKeys / 2.0);

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

void BStarTree::Root::join(int minKeys) {}

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

		auto spilled = curr->spill(allowedKeys);
		if (spilled)
			break;
		else {
			curr->split(allowedKeys);
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
		auto allowedKeys = curr == root ? 2 : MIN_NODE_KEYS;
		if (curr->keyCount() >= allowedKeys)
			break;

		auto right = curr->getRight(), left = curr->getLeft();
		Node* sibling = nullptr;

		if (right && right->keyCount() >= MIN_NODE_KEYS + 1)
			sibling = right;
		else if (left && left->keyCount() >= MIN_NODE_KEYS + 1)
			sibling = left;

		if (sibling) {
			sibling->spillInto(curr);
			break;
		}

		curr->join(allowedKeys);
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
			return curr.getKey();

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

			auto parentNodeIndex = parentNode->getIndexInParent();

			position = { parentNode, parentNodeIndex };
		}

		return ++position;
	} else {
		position++;
		while (!position.node->isLeaf())
			position = { position.node->children[position.index], 0 };

		return position;
	}
}
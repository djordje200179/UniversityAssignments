#include "Node.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <iomanip>

using namespace std;

BStarTree::Node::Node() : parent(nullptr), level(0) {
	children.push_back(nullptr);
}

BStarTree::Node::Node(Node* parent, const vector<string>& initKeys, const vector<Node*>& initChildren)
	: parent(parent), children(initChildren), keys(initKeys) {
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

	int keyMid = allKeys.size() / 2;
	int childrenMid = (allChildren.size() + 1) / 2;

	this->keys = vector<string>(allKeys.begin(), allKeys.begin() + keyMid);
	parent->keys[index1] = allKeys[keyMid];
	parent->keys.erase(parent->keys.begin() + index1 + 1);
	right->keys = vector<string>(allKeys.begin() + keyMid + 1, allKeys.end());
	
	this->children = vector<Node*>(allChildren.begin(), allChildren.begin() + childrenMid);
	right->children = vector<Node*>(allChildren.begin() + childrenMid, allChildren.end());

	delete left;
	parent->children.erase(parent->children.begin() + index1);

	this->updateChildren();
	right->updateChildren();

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

	int keyMid = allKeys.size() / 2;
	int childrenMid = (allChildren.size() + 1) / 2;

	left->keys = vector<string>(allKeys.begin(), allKeys.begin() + keyMid);
	divider = allKeys[keyMid];
	right->keys = vector<string>(allKeys.begin() + keyMid + 1, allKeys.end());

	left->children = vector<Node*>(allChildren.begin(), allChildren.begin() + childrenMid);
	right->children = vector<Node*>(allChildren.begin() + childrenMid, allChildren.end());

	left->updateChildren();
	right->updateChildren();
}

bool BStarTree::Node::trySpill(SpillType type) {
	auto right = getRight(), left = getLeft();
	auto right2 = right ? right->getRight() : nullptr;
	auto left2 = left ? left->getLeft() : nullptr;

	Node* sibling = nullptr;
	Node* over = nullptr;

	if (type == SpillType::OVER) {
		int maxKeys = keys.size() - 1;
		if (right && right->keyCount() < maxKeys)
			sibling = right;
		else if (left && left->keyCount() < maxKeys)
			sibling = left;
	} else {
		int minKeys = keys.size() + 1;
		if (right && right->keyCount() > minKeys)
			sibling = right;
		else if (left && left->keyCount() > minKeys)
			sibling = left;
		else if (right2 && right2->keyCount() > minKeys)
			sibling = right2, over = right;
		else if (left2 && left2->keyCount() > minKeys)
			sibling = left2, over = left;
	}


	if (!sibling)
		return false;

	if (type == SpillType::OVER)
		spill(this, sibling);
	else {
		if(!over)
			spill(sibling, this);
		else {
			spill(over, sibling);
			spill(this, over);
		}
	}

	return true;
}

void BStarTree::Node::updateChildren() {
	for (auto child : children)
		if (child)
			child->parent = this;
}
#include "Root.hpp"
#include <vector>
#include <string>

using namespace std;

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
	auto left = children[0];
	auto right = children[1];

	vector<string> allKeys;
	vector<Node*> allChildren;

	allKeys.insert(allKeys.end(), left->keys.begin(), left->keys.end());
	allKeys.insert(allKeys.end(), keys[0]);
	allKeys.insert(allKeys.end(), right->keys.begin(), right->keys.end());

	allChildren.insert(allChildren.end(), left->children.begin(), left->children.end());
	allChildren.insert(allChildren.end(), right->children.begin(), right->children.end());

	keys = allKeys;
	children = allChildren;

	updateChildren();

	delete left;
	delete right;
}
#include "Dictionary.hpp"
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

Dictionary::Dictionary() {
	root = nullptr;
}

Dictionary::~Dictionary() {
	auto traversalStack = stack<Node*>();
	auto postorderStack = stack<Node*>();

	traversalStack.push(root);
	while(!traversalStack.empty()) {
		auto node = traversalStack.top();
		traversalStack.pop();

		if(!node)
			continue;

		postorderStack.push(node);

		traversalStack.push(node->left);
		traversalStack.push(node->right);
	}

	while(!postorderStack.empty()) {
		auto node = postorderStack.top();
		postorderStack.pop();

		delete node;
	}
}

void Dictionary::addValue(const string& key, const string& value) {
	auto newNode = new Node(key, value);

	if(!root) {
		root = newNode;

		return;
	}

	auto curr = root;
	auto prev = (Node*)nullptr;

	while(curr) {
		prev = curr;

		if(curr->key == key) {
			if(!curr->left)
				curr->left = newNode;
			else {
				curr = getPreviousNode(curr);

				curr->right = newNode;
			}

			newNode->parent = curr;
			newNode->level = curr->level + 1;

			return;
		} else if(curr->key > key)
			curr = curr->left;
		else
			curr = curr->right;
	}

	if(prev->key > key)
		prev->left = newNode;
	else
		prev->right = newNode;

	newNode->parent = prev;
	newNode->level = prev->level + 1;
}

void Dictionary::addValues(const vector<pair<string, string>>& words) {
	for(auto& word : words)
		addValue(word.first, word.second);
}

vector<string> Dictionary::getValues(const string& key) {
	auto node = getNode(key);

	if(!node)
		return vector<string>();

	auto words = vector<string>();

	for(auto temp = node; temp && temp->key == key; temp = getPreviousNode(temp))
		words.push_back(temp->value);

	return words;
}

string Dictionary::getSpecialKey(const string& prefix) {
	auto curr = root;
	while(curr) {
		if(curr->key.find(prefix) == 0) 
			break;
		else if(curr->key > prefix)
			curr = curr->left;
		else
			curr = curr->right;
	}

	if(!curr)
		return string();

	auto prev = curr;
	do {
		prev = curr;
		curr = getPreviousNode(curr);
	} while(curr && curr->key.find(prefix) == 0);

	return prev->key;
}

void Dictionary::deleteValues(const string& key) {
	while(true) {
		auto node = getNode(key);

		if(!node)
			break;

		if(node->left && node->right) {
			auto prev = getPreviousNode(node);

			swap(prev->key, node->key);
			swap(prev->value, node->value);

			node = prev;
		}

		auto parent = node->parent;

		if(!node->left && !node->right) {
			if(node == root)
				root = nullptr;
			else if(parent->left == node)
				parent->left = nullptr;
			else
				parent->right = nullptr;
		} else {
			auto child = (node->left) ? node->left : node->right;

			if(node == root)
				root = child;
			else
				if(node == parent->left)
					parent->left = child;
				else
					parent->right = child;

			child->level--;
			child->parent = parent;

			updateLevels(child);
		}

		delete node;
	}
}

void Dictionary::printTree() {
	auto traversalStack = stack<Node*>();

	traversalStack.push(root);

	while(!traversalStack.empty()) {
		auto node = traversalStack.top();
		traversalStack.pop();

		if(!node)
			continue;

		for(unsigned short i = 0; i < node->level; i++)
			cout << "  ";

		if(node->parent)
			cout << (node->parent->left == node ? 'L' : 'R') << ':';
		cout << node->key << endl;

		traversalStack.push(node->right);
		traversalStack.push(node->left);
	}
}

Node* Dictionary::getNode(const string& key) {
	auto curr = root;
	while(curr)
		if(curr->key == key)
			break;
		else if(curr->key > key)
			curr = curr->left;
		else
			curr = curr->right;

	return curr;
}

void Dictionary::updateLevels(Node* startNode) {
	auto traversalStack = stack<Node*>();

	traversalStack.push(root);

	while(!traversalStack.empty()) {
		auto node = traversalStack.top();
		traversalStack.pop();

		if(!node)
			continue;

		node->level = node->parent ? node->parent->level + 1 : 0;

		traversalStack.push(node->right);
		traversalStack.push(node->left);
	}
}

Node* Dictionary::getPreviousNode(Node* node) {
	if(!node->parent)
		return nullptr;

	auto curr = node;

	if(curr->left) {
		curr = curr->left;

		while(curr->right)
			curr = curr->right;

		return curr;
	} else {
		auto prev = curr->parent;

		while(prev && curr == prev->left)
			curr = prev, prev = prev->parent;

		return prev;
	}
}
#include "DynamicHashTable.hpp"
#include "Student.hpp"
#include <stack>
#include <algorithm>

using namespace std;

DynamicHashTable::Node::~Node() = default;

DynamicHashTable::DynamicHashTable(int bucketSize, int hashDegree, int rootAddressBits) :
	HashTable(bucketSize, hashDegree),
	rootAddressBits(rootAddressBits),
	buckets(1ull << rootAddressBits) {
	for (auto& node : buckets)
		node = new LeafNode;
}

std::vector<bool> DynamicHashTable::calculateAdress(unsigned int key) const {
	auto hash = hashFunction(key);
	vector<bool> bits(hashDegree);

	for (int i = hashDegree - 1; i >= 0; i--) {
		bits[i] = hash & 1;
		hash >>= 1;
	}

	return bits;
}

DynamicHashTable::Node*& DynamicHashTable::getRootNode(const std::vector<bool>&bits) const {
	size_t index = 0;

	for (int i = rootAddressBits - 1; i >= 0; i--) {
		index <<= 1;
		index |= bits[i];
	}

	return buckets[index];
}

DynamicHashTable::LeafNode* DynamicHashTable::getBucket(unsigned int key) const {
	auto bits = calculateAdress(key);
	auto currNode = getRootNode(bits);
	auto currIndex = rootAddressBits;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto internalNode = dynamic_cast<InternalNode*>(currNode))
			currNode = (bits[currIndex] == 0) ? internalNode->left : internalNode->right;
		else
			break;
	}

	auto leaf = dynamic_cast<LeafNode*>(currNode);

	if (!leaf)
		throw std::runtime_error("Leaf doesn't exist");

	return leaf;
}

void DynamicHashTable::print(std::ostream & os) const {
	//implementirati ispis
}

Student* DynamicHashTable::findKey(unsigned int key) const {
	auto bits = calculateAdress(key);
	auto currNode = getRootNode(bits);
	auto currIndex = rootAddressBits;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto internalNode = dynamic_cast<InternalNode*>(currNode))
			currNode = (bits[currIndex] == 0) ? internalNode->left : internalNode->right;
		else
			break;
	}

	auto leaf = dynamic_cast<LeafNode*>(currNode);

	if (!leaf)
		throw std::runtime_error("Leaf doesn't exist");

	auto it = find_if(leaf->entries.begin(), leaf->entries.end(), [key](Student* entry) { return key == entry->getId(); });
	return it != leaf->entries.end() ? *it : nullptr;
}

bool DynamicHashTable::insertKey(unsigned int key, Student* data) {
	if (findKey(key))
		return false;

	auto bits = calculateAdress(key);
	auto currNode = getRootNode(bits);
	auto currIndex = rootAddressBits;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto internalNode = dynamic_cast<InternalNode*>(currNode))
			currNode = (bits[currIndex] == 0) ? internalNode->left : internalNode->right;
		else
			break;
	}

	auto currLeaf = dynamic_cast<LeafNode*>(currNode);

	if (!currLeaf)
		throw std::runtime_error("Leaf doesn't exist");

	currLeaf->entries.push_back(data);

	while (currLeaf->entries.size() > bucketSize) {
		if (currIndex == hashDegree) {
			auto it = find_if(currLeaf->entries.begin(), currLeaf->entries.end(), [key](Student* student) {
				return student->getId() == key;
			});

			if (it == currLeaf->entries.end())
				throw std::runtime_error("Element not added");

			currLeaf->entries.erase(it);

			return false;
		}

		auto leftNode = new LeafNode;
		auto rightNode = new LeafNode;
		auto newParentNode = new InternalNode(leftNode, rightNode);

		for (auto& data : currLeaf->entries) {
			auto bits = calculateAdress(data->getId());
			(bits[currIndex] == false ? leftNode : rightNode)->entries.push_back(data);
		}

		if (!currLeaf->parent) {
			delete currLeaf;
			getRootNode(bits) = newParentNode;
		} else {
			auto parent = currLeaf->parent;
			(parent->left == currLeaf ? parent->left : parent->right) = newParentNode;
			newParentNode->parent = parent;

			delete currLeaf;
		}

		currLeaf = (leftNode->entries.size() > rightNode->entries.size() ? leftNode : rightNode);
		currIndex++;
	}

	return true;
}

bool DynamicHashTable::deleteKey(unsigned int key, bool callDestructor) {
	auto bits = calculateAdress(key);
	auto currNode = getRootNode(bits);
	auto currIndex = rootAddressBits;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto internalNode = dynamic_cast<InternalNode*>(currNode))
			currNode = (bits[currIndex] == 0) ? internalNode->left : internalNode->right;
		else
			break;
	}

	auto currLeaf = dynamic_cast<LeafNode*>(currNode);

	if (!currLeaf)
		throw std::runtime_error("Leaf doesn't exist");

	auto it = find_if(currLeaf->entries.begin(), currLeaf->entries.end(), [key](Student* entry) { return key == entry->getId(); });
	if (it == currLeaf->entries.end())
		return false;

	currLeaf->entries.erase(it);

	while (true) {
		InternalNode* parent = currLeaf->parent;
		if (!parent)
			break;

		Node* siblingNode = parent->left == currLeaf ? parent->right : parent->left;
		LeafNode* siblingLeaf = dynamic_cast<LeafNode*>(siblingNode);

		if (!siblingLeaf || siblingLeaf->entries.size() + currLeaf->entries.size() > bucketSize)
			break;

		auto newLeaf = new LeafNode;

		newLeaf->entries.insert(newLeaf->entries.end(), currLeaf->entries.begin(), currLeaf->entries.end());
		newLeaf->entries.insert(newLeaf->entries.end(), siblingLeaf->entries.begin(), siblingLeaf->entries.end());

		auto grandparent = parent->parent;
		if (!grandparent)
			getRootNode(bits) = newLeaf;
		else
			(grandparent->right == parent ? grandparent->right : grandparent->left) = newLeaf;
		newLeaf->parent = grandparent;

		delete parent;
		delete currLeaf;
		delete siblingLeaf;

		currLeaf = newLeaf;
	}

	return true;
}

void DynamicHashTable::clear() {
	stack<Node*> traversalStack;
	stack<Node*> deletingStack;

	for (auto node : buckets)
		traversalStack.push(node);

	while (!traversalStack.empty()) {
		auto node = traversalStack.top();
		traversalStack.pop();

		deletingStack.push(node);

		if (auto internalNode = dynamic_cast<InternalNode*>(node)) {
			traversalStack.push(internalNode->right);
			traversalStack.push(internalNode->left);
		}
	}

	while (!deletingStack.empty()) {
		auto node = deletingStack.top();
		deletingStack.pop();

		delete node;
	}

	for (auto& node : buckets)
		node = new LeafNode;
}

size_t DynamicHashTable::keyCount() const {
	stack<Node*> traversalStack;

	for (auto node : buckets)
		traversalStack.push(node);

	size_t counter = 0;
	while (!traversalStack.empty()) {
		auto treeNode = traversalStack.top();
		traversalStack.pop();

		if (auto node = dynamic_cast<InternalNode*>(treeNode)) {
			traversalStack.push(node->right);
			traversalStack.push(node->left);
		} else if (auto leaf = dynamic_cast<LeafNode*>(treeNode))
			counter += leaf->entries.size();
	}

	return counter;
}

size_t DynamicHashTable::tableSize() const {
	stack<Node*> traversalStack;

	for (auto node : buckets)
		traversalStack.push(node);

	size_t counter = 0;
	while (!traversalStack.empty()) {
		auto treeNode = traversalStack.top();
		traversalStack.pop();

		if (auto node = dynamic_cast<InternalNode*>(treeNode)) {
			traversalStack.push(node->right);
			traversalStack.push(node->left);
		} else if (auto leaf = dynamic_cast<LeafNode*>(treeNode))
			counter++;
	}

	return counter;
}

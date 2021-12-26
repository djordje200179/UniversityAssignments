#include "DynamicHashTable.hpp"
#include "Student.hpp"
#include <numeric>
#include <stack>

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

DynamicHashTable::Node*& DynamicHashTable::getBucket(const std::vector<bool>&bits) const {
	size_t index = 0;

	for (int i = rootAddressBits - 1; i >= 0; i--) {
		index <<= 1;
		index |= bits[i];
	}

	return buckets[index];
}

void DynamicHashTable::print(std::ostream & os) const {
	//implementirati ispis
}

Student* DynamicHashTable::findKey(unsigned int key) const {
	auto bits = calculateAdress(key);
	auto currNode = getBucket(bits);
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

bool DynamicHashTable::insertKey(unsigned int key, Student * data) {
	auto bits = calculateAdress(key);
	auto currNode = getBucket(bits);
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

	if (currIndex == hashDegree && currLeaf->entries.size() == bucketSize)
		return false;

	currLeaf->entries.push_back(data);

	while (currLeaf->entries.size() == bucketSize + 1) {
		auto leftNode = new LeafNode;
		auto rightNode = new LeafNode;
		auto newParentNode = new InternalNode(leftNode, rightNode);

		for (auto& data : currLeaf->entries) {
			auto bits = calculateAdress(data->getId());
			(bits[currIndex] == false ? leftNode : rightNode)->entries.push_back(data);
		}

		if (!currLeaf->parent) {
			delete currLeaf;
			getBucket(bits) = newParentNode;
		} else {
			auto parent = currLeaf->parent;
			(parent->left == currLeaf ? parent->left : parent->right) = newParentNode;
			newParentNode->parent = parent;

			delete currLeaf;
		}
			
		currLeaf = (leftNode->entries.size() > rightNode->entries.size() ? leftNode : rightNode);
	}

	return true;
}

bool DynamicHashTable::deleteKey(unsigned int key, bool callDestructor) {
	auto bits = calculateAdress(key);
	auto currNode = getBucket(bits);
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

	auto parent = currLeaf->parent;
	if (!parent)
		return true;

	auto siblingNode = parent->left == currLeaf ? parent->right : parent->left;
	auto siblingLeaf = dynamic_cast<LeafNode*>(siblingNode);

	if (!siblingLeaf)
		return true;

	if (siblingLeaf->entries.size() + currLeaf->entries.size() > bucketSize)
		return true;

	// implementirati spajanje

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
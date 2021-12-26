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
		index |= bits[i];
		index <<= 1;
	}

	return buckets[index];
}

void DynamicHashTable::print(std::ostream & os) const {
	//implementirati ispis
}

Student* DynamicHashTable::findKey(unsigned int key) const {
	auto bits = calculateAdress(key);
	auto node = getBucket(bits);
	auto currIndex = rootAddressBits;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto internalNode = dynamic_cast<InternalNode*>(node))
			node = (bits[currIndex] == 0) ? internalNode->left : internalNode->right;
		else
			break;
	}

	auto leaf = dynamic_cast<LeafNode*>(node);

	if (!leaf)
		throw std::runtime_error("Leaf doesn't exist");

	for (auto entry : leaf->entries)
		if (key == entry->getId())
			return entry;

	return nullptr;
}

bool DynamicHashTable::insertKey(unsigned int key, Student * data) {
	auto bits = calculateAdress(key);
	auto node = getBucket(bits);
	auto currIndex = rootAddressBits;
	InternalNode* parentNode = nullptr;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto internalNode = dynamic_cast<InternalNode*>(node)) {
			parentNode = internalNode;
			node = (bits[currIndex] == 0) ? internalNode->left : internalNode->right;
		} else
			break;
	}

	auto leaf = dynamic_cast<LeafNode*>(node);

	if (!leaf)
		throw std::runtime_error("Leaf doesn't exist");

	if (currIndex == hashDegree && leaf->entries.size() == bucketSize)
		return false;

	leaf->entries.push_back(data);

	while (leaf->entries.size() == bucketSize + 1) {
		auto leftNode = new LeafNode;
		auto rightNode = new LeafNode;
		auto newParentNode = new InternalNode(leftNode, rightNode);

		for (auto& data : leaf->entries) {
			auto bits = calculateAdress(data->getId());
			(bits[currIndex] == false ? leftNode : rightNode)->entries.push_back(data);
		}

		delete leaf;

		if (!parentNode)
			getBucket(bits) = newParentNode;
		else 
			(parentNode->left == leaf ? parentNode->left : parentNode->right) = newParentNode;

		leaf = (leftNode->entries.size() > rightNode->entries.size() ? leftNode : rightNode);
	}

	return true;
}

bool DynamicHashTable::deleteKey(unsigned int key, bool callDestructor) {
	auto bits = calculateAdress(key);
	auto node = getBucket(bits);

	//implementirati brisanje

	return false;
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
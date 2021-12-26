#include "DynamicHashTable.hpp"
#include "Student.hpp"
#include <numeric>
#include <stack>

using namespace std;

DynamicHashTable::TreeNode::~TreeNode() = default;

DynamicHashTable::DynamicHashTable(int bucketSize, int hashDegree, int rootAddressBits) :
	HashTable(bucketSize, hashDegree),
	rootAddressBits(rootAddressBits),
	buckets(1ull << rootAddressBits) {
	for (auto& bucket : buckets)
		bucket = new Leaf;
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

DynamicHashTable::TreeNode*& DynamicHashTable::getBucket(const std::vector<bool>&bits) const {
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
		if (auto treeNode = dynamic_cast<Node*>(node))
			node = (bits[currIndex] == 0) ? treeNode->left : treeNode->right;
		else
			break;
	}

	auto leaf = dynamic_cast<Leaf*>(node);

	if (!leaf)
		throw std::runtime_error("Leaf doesn't exist");

	for (auto data : leaf->entries)
		if (key == data->getId())
			return data;

	return nullptr;
}

bool DynamicHashTable::insertKey(unsigned int key, Student * data) {
	auto bits = calculateAdress(key);
	auto node = getBucket(bits);
	auto currIndex = rootAddressBits;
	Node* parentNode = nullptr;

	for (currIndex = rootAddressBits; currIndex < hashDegree; currIndex++) {
		if (auto treeNode = dynamic_cast<Node*>(node)) {
			parentNode = treeNode;
			node = (bits[currIndex] == 0) ? treeNode->left : treeNode->right;
		} else
			break;
	}

	auto leaf = dynamic_cast<Leaf*>(node);

	if (!leaf)
		throw std::runtime_error("Leaf doesn't exist");

	if (currIndex == hashDegree && leaf->entries.size() == bucketSize)
		return false;

	leaf->entries.push_back(data);

	while (leaf->entries.size() == bucketSize + 1) {
		auto leftNode = new Leaf;
		auto rightNode = new Leaf;
		auto newParentNode = new Node(leftNode, rightNode);

		for (auto& data : leaf->entries) {
			auto bits = calculateAdress(data->getId());
			(bits[currIndex] == false ? leftNode : rightNode)->entries.push_back(data);
		}

		delete leaf;

		if (!parentNode)
			getBucket(bits) = newParentNode;
		else
			(parentNode->left == leaf ? parentNode->left : parentNode->right) = newParentNode;

		leaf = (leftNode->entries.size() > 0 ? leftNode : rightNode);
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
	stack<TreeNode*> traversalStack;
	stack<TreeNode*> deletingStack;

	for (auto node : buckets)
		traversalStack.push(node);

	while (!traversalStack.empty()) {
		auto treeNode = traversalStack.top();
		traversalStack.pop();

		deletingStack.push(treeNode);

		if (auto node = dynamic_cast<Node*>(treeNode)) {
			traversalStack.push(node->right);
			traversalStack.push(node->left);
		}
	}

	while (!deletingStack.empty()) {
		auto treeNode = deletingStack.top();
		deletingStack.pop();

		delete treeNode;
	}

	for (auto& bucket : buckets)
		bucket = new Leaf;
}

size_t DynamicHashTable::keyCount() const {
	stack<TreeNode*> traversalStack;

	for (auto node : buckets)
		traversalStack.push(node);

	size_t counter = 0;
	while (!traversalStack.empty()) {
		auto treeNode = traversalStack.top();
		traversalStack.pop();

		if (auto node = dynamic_cast<Node*>(treeNode)) {
			traversalStack.push(node->right);
			traversalStack.push(node->left);
		} else if (auto leaf = dynamic_cast<Leaf*>(treeNode))
			counter += leaf->entries.size();
	}

	return counter;
}
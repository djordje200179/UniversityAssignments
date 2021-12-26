#include "DynamicHashTable.hpp"
#include "Student.hpp"
#include <numeric>
#include <stack>

using namespace std;

DynamicHashTable::TreeNode::~TreeNode() = default;

DynamicHashTable::DynamicHashTable(int bucketSize, int hashDegree, int rootBuckets) :
	HashTable(bucketSize, hashDegree),
	b(rootBuckets),
	buckets(1ull << rootBuckets) {
	for (auto& bucket : buckets)
		bucket = new Leaf;
}

std::vector<bool> DynamicHashTable::calculateAdress(unsigned int key) const {
	auto hash = hashFunction(key);
	vector<bool> bits(hashDegree);

	for (int i = 0; i < hashDegree; i++) {
		bits[hashDegree - i - 1] = hash & 1;
		hash >>= 1;
	}

	return bits;
}

DynamicHashTable::TreeNode*& DynamicHashTable::getBucket(const std::vector<bool>&bits) const {
	size_t index = 0;
	size_t term;
	for (int i = 0, term = 1 << (b - 1); i < b; i++) {
		index += term * bits[i];
		term >>= 1;
	}

	return buckets[index];
}

void DynamicHashTable::print(std::ostream & os) const {
	//popraviti ispis
}

Student* DynamicHashTable::findKey(unsigned int key) const {
	auto bits = calculateAdress(key);
	auto node = getBucket(bits);
	auto currIndex = b;

	for (currIndex = b; currIndex < hashDegree; currIndex++) {
		if (auto treeNode = dynamic_cast<Node*>(node))
			node = (bits[currIndex] == 0) ? treeNode->left : treeNode->right;
		else
			break;
	}

	auto leaf = dynamic_cast<Leaf*>(node);

	for (auto data : leaf->entries)
		if (key == data->getId())
			return data;

	return nullptr;
}

bool DynamicHashTable::insertKey(unsigned int key, Student * data) {
	auto bits = calculateAdress(key);
	auto node = getBucket(bits);
	auto currIndex = b;
	Node* parentNode = nullptr;

	for (currIndex = b; currIndex < hashDegree; currIndex++) {
		if (auto treeNode = dynamic_cast<Node*>(node)) {
			parentNode = treeNode;
			node = (bits[currIndex] == 0) ? treeNode->left : treeNode->right;
		} else
			break;
	}

	auto leaf = dynamic_cast<Leaf*>(node);

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
	auto bucket = getBucket(bits);

	return false; //popraviti brisanje
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
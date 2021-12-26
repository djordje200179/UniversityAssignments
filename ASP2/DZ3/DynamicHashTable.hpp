#pragma once

#include "HashTable.hpp"
#include <vector>

class DynamicHashTable : public HashTable {
private:
	struct Node {
		virtual ~Node() = 0;
	};

	struct InternalNode : Node {
		InternalNode(Node* left, Node* right) : left(left), right(right) {}

		Node* left;
		Node* right;
	};

	struct LeafNode : Node {
		std::vector<Student*> entries;
	};
private:
	const int rootAddressBits;
	mutable std::vector<Node*> buckets;

	std::vector<bool> calculateAdress(unsigned int key) const;
	Node*& getBucket(const std::vector<bool>& bits) const;
protected:
	void print(std::ostream& os) const override;
public:
	DynamicHashTable(int bucketSize, int hashDegree, int rootBuckets);
	~DynamicHashTable() { clear(); }

	Student* findKey(unsigned int key) const override;
	bool insertKey(unsigned int key, Student* data) override;
	bool deleteKey(unsigned int key, bool callDestructor = false) override;

	void clear() override;
	size_t keyCount() const override;
};
#pragma once

#include "HashTable.hpp"
#include <vector>

class DynamicHashTable : public HashTable {
private:
	struct TreeNode {};

	struct Node : TreeNode {
		TreeNode* left;
		TreeNode* right;
	};

	struct Leaf : TreeNode {
		std::vector<Student*> entries;
	};
private:
	std::vector<TreeNode*> buckets;
protected:
	void print(std::ostream& os) const override;
public:
	DynamicHashTable(int bucketSize, int hashDegree, int rootBuckets);

	Student* findKey(unsigned int key) const override;
	bool insertKey(unsigned int key, Student* data) override;
	bool deleteKey(unsigned int key, bool callDestructor = false) override;

	void clear() override;
	size_t keyCount() const override;
};
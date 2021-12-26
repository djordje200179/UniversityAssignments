#pragma once

#include "HashTable.hpp"
#include <vector>

class DynamicHashTable : public HashTable {
private:
	struct TreeNode {
		virtual ~TreeNode() = 0;
	};

	struct Node : TreeNode {
		Node(TreeNode* left, TreeNode* right) : left(left), right(right) {}

		TreeNode* left;
		TreeNode* right;
	};

	struct Leaf : TreeNode {
		std::vector<Student*> entries;
	};
private:
	const int b;
	mutable std::vector<TreeNode*> buckets;

	std::vector<bool> calculateAdress(unsigned int key) const;
	TreeNode*& getBucket(const std::vector<bool>& bits) const;
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
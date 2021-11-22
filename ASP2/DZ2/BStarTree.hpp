#ifndef B_STAR_TREE_H
#define B_STAR_TREE_H

#include "Node.hpp"
#include <ostream>

class BStarTree {
private:
	using CStr = const std::string&;
	struct Position {
		Node* node;
		int index;

		operator bool() const { return node != nullptr; }
		operator CStr() const { return node->keys[index]; }
		void operator++() { index++; }
		void operator++(int) { index++; }
	};
public:
	BStarTree(int degree);
	~BStarTree();

	bool keyExists(CStr key) const;
	bool addKey(CStr key);
	bool removeKey(CStr key);
	void printTree(std::ostream& os) const;
	CStr findKthKey(int k) const;

	bool operator()(CStr key) const { return keyExists(key); }
	bool operator+=(CStr key) { return addKey(key); }
	bool operator-=(CStr key) { return removeKey(key); }
	friend std::ostream& operator<<(std::ostream& os, const BStarTree& tree) { tree.printTree(os); return os; }
	CStr operator()(int k) const { return findKthKey(k); }
private:
	Position findKey(CStr key) const;

	Node* root = nullptr;
	const int degree;
	const int maxKeys;
	const int minNodeKeys;
	const int maxRootKeys;
};

#endif // B_STAR_TREE_H
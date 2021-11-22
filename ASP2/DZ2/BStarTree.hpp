#ifndef B_STAR_TREE_H
#define B_STAR_TREE_H

#include "Node.hpp"
#include <ostream>

class BStarTree {
private:
	using CStr = const std::string&;
public:
	BStarTree(int degree);
	~BStarTree();

	bool keyExists(CStr key) const;
	bool addKey(CStr key);
	bool removeKey(CStr key);
	void printTree(std::ostream& os) const;
	CStr findKthWord(size_t n) const;

	bool operator()(CStr key) const { return keyExists(key); }
	bool operator+=(CStr key) { return addKey(key); }
	bool operator-=(CStr key) { return removeKey(key); }
	friend std::ostream& operator<<(std::ostream& os, const BStarTree& tree) { tree.printTree(os); return os; }
	CStr operator()(size_t n) const { return findKthWord(n); }
private:
	Node* root = nullptr;
	const int degree;
	const int maxKeys;
	const int minNodeKeys;
	const int maxRootKeys;
};

#endif // B_STAR_TREE_H
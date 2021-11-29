#pragma once

#include "Misc.hpp"
#include "Node.hpp"
#include <ostream>
#include <istream>
#include <vector>
#include <string>

namespace BStarTree {
class Tree {
public:
	Tree(int degree)
		: DEGREE(degree), MAX_NODE_KEYS(degree - 1),
		MAX_ROOT_KEYS(2 * floor((2 * degree - 2) / 3.0)),
		MIN_NODE_KEYS(ceil((2 * degree - 1) / 3.0 - 1)) {};

	~Tree();

	bool keyExists(CStr key) const;
	bool addKey(CStr key);
	bool removeKey(CStr key);
	void printTree(std::ostream& os) const;
	void inputWords(std::istream& is);
	CStr findKthKey(int k) const;

	bool operator()(CStr key) const { return keyExists(key); }
	CStr operator()(int k) const { return findKthKey(k); }
	bool operator+=(CStr key) { return addKey(key); }
	bool operator-=(CStr key) { return removeKey(key); }
	friend std::ostream& operator<<(std::ostream& os, const Tree& tree) { tree.printTree(os); return os; }
	friend std::istream& operator>>(std::istream& is, Tree& tree) { tree.inputWords(is); return is; }
private:
	Position findKey(CStr key) const;
	Position findSuccessor(Position pos) const;

	Node* root = nullptr;
	const int DEGREE;
	const int MAX_NODE_KEYS;
	const int MAX_ROOT_KEYS;
	const int MIN_NODE_KEYS;
};
}
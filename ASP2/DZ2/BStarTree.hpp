#ifndef B_STAR_TREE_H
#define B_STAR_TREE_H

#include <ostream>
#include <vector>
#include <string>

class BStarTree {
private:
	using CStr = const std::string&;

	struct Node;

	struct Position {
		Node* node;
		int index;

		operator bool() const { return node != nullptr; }
		operator CStr() const { return node->keys[index]; }
		Position& operator++() { index++; return *this; }
		Position operator++(int) { auto copy = *this; index++; return copy; }
	};

	struct Node {
		std::vector<std::string> keys;
		std::vector<Node*> children;
		Position positionInParent;

		Node(Position positionInParent = { nullptr, 0 });

		bool isLeaf();
		bool canAddKey(int maxKeys);
		void addKey(CStr key);
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
	Position findSuccessor(Position pos);

	Node* root = nullptr;
	const int DEGREE;
	const int MAX_KEYS;
	const int MAX_ROOT_KEYS;
	const int MIN_NODE_KEYS;
};

#endif // B_STAR_TREE_H
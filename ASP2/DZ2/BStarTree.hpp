#ifndef B_STAR_TREE_H
#define B_STAR_TREE_H

#include <ostream>
#include <istream>
#include <vector>
#include <string>

class BStarTree {
private:
	friend void testPointers(const BStarTree& tree);

	using CStr = const std::string&;

	struct Node;

	struct Position {
		Node* node;
		int index;

		std::string& getKey() const { return node->keys[index]; }
		operator bool() const { return node != nullptr; }
		Position& operator++() { index++; return *this; }
		Position operator++(int) { auto copy = *this; index++; return copy; }
	};

	struct Node {
		std::vector<std::string> keys;
		std::vector<Node*> children;
		Node* parent;
		int level = -1;
	
		Node(Node* parent, 
			 const std::vector<std::string>& initKeys,
			 const std::vector<Node*>& initChildren);

		bool isLeaf() { return children.front() == nullptr; }
		int keyCount() { return keys.size(); }
		void addKey(CStr key);
		void removeKey(int index);
		int getIndexInParent();
		Node* getLeft();
		Node* getRight();
		void print(std::ostream& os) const;
		virtual void split(int maxKeys);
		void spillInto(Node* sibling);
		virtual bool spill(int maxKeys);
		void updateChildren();
	protected:
		Node();
	};

	struct Root : Node {
		Root() = default;

		void split(int maxKeys) override;
		bool spill(int maxKeys) override { return false; }
	};
public:
	BStarTree(int degree) 
		: DEGREE(degree), MAX_NODE_KEYS(degree - 1),
		MAX_ROOT_KEYS(2 * floor((2 * degree - 2) / 3.0)),
		MIN_NODE_KEYS(ceil((2 * degree - 1) / 3.0 - 1)) {};

	~BStarTree();

	bool keyExists(CStr key) const;
	bool addKey(CStr key);
	bool removeKey(CStr key);
	void printTree(std::ostream& os) const;
	void inputWords(std::istream& is);
	CStr findKthKey(int k) const;

	bool operator()(CStr key) const { return keyExists(key); }
	bool operator+=(CStr key) { return addKey(key); }
	bool operator-=(CStr key) { return removeKey(key); }
	friend std::ostream& operator<<(std::ostream& os, const BStarTree& tree) { tree.printTree(os); return os; }
	friend std::istream& operator>>(std::istream& is, BStarTree& tree) { tree.inputWords(is); return is; }
	CStr operator()(int k) const { return findKthKey(k); }
private:
	Position findKey(CStr key) const;
	Position findSuccessor(Position pos);

	Node* root = nullptr;
	const int DEGREE;
	const int MAX_NODE_KEYS;
	const int MAX_ROOT_KEYS;
	const int MIN_NODE_KEYS;
};

#endif // B_STAR_TREE_H
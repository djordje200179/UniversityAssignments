#pragma once

#include "Misc.hpp"
#include <string>
#include <vector>

namespace BStarTree {
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
	virtual bool trySpill(SpillType type);
	virtual void split();
	virtual void join();
	static void spill(Node* from, Node* to);
	void updateChildren();
protected:
	Node();
};
}
#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

struct Node {
	std::vector<std::string> keys;
	std::vector<Node*> nodes;

	bool isLeaf();
};

#endif // NODE_H
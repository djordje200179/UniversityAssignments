#pragma once

#include <string>
#include <vector>
#include <utility>
#include <ostream>
#include "Node.hpp"

class Dictionary {
public:
	Dictionary();
	~Dictionary();

	void addValue(const std::string& key, const std::string& value);
	void addValues(const std::vector<std::pair<std::string, std::string>>& words);

	std::vector<std::string> getValues(const std::string& key);
	std::string getSpecialKey(const std::string& prefix);
	void printTree();

	void deleteValues(const std::string& key);
private:
	Node* getPreviousNode(Node* node);
	Node* getNode(const std::string& key);

	void updateLevels(Node* startNode);

	Node* root;
};
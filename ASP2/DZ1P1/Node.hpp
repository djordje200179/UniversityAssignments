#pragma once

#include <string>

struct Node {
	Node(const std::string& key, const std::string& value, unsigned short level = 0);

	unsigned short level;

	Node* left;
	Node* right;
	Node* parent;

	std::string key;
	std::string value;
};
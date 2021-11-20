#include "Node.hpp"

Node::Node(const std::string& key, const std::string& value, unsigned short level) : key(key), value(value) {
	this->level = level;

	left = nullptr;
	right = nullptr;
	parent = nullptr;
}
#include "Misc.hpp"
#include "Node.hpp"

std::string& BStarTree::Position::getKey() const { 
	return node->keys[index];
}

BStarTree::Position::operator bool() const { 
	return node != nullptr;
}

BStarTree::Position& BStarTree::Position::operator++() {
	index++; return *this; 
}

BStarTree::Position BStarTree::Position::operator++(int) { 
	auto copy = *this; 
	index++; 
	return copy; 
}
#pragma once

#include <string>

namespace BStarTree {
using CStr = const std::string&;

struct Position {
	struct Node* node;
	int index;

	std::string& getKey() const;
	operator bool() const;
	Position& operator++();
	Position operator++(int);
};

enum class SpillType : bool {
	UNDER, OVER
};
}
#include <iostream>
#include "BStarTree.hpp"

int main() {
	BStarTree tree(4);

	tree += "marko";
	tree += "janko";
	tree += "putin";
	tree += "darko";
	tree += "jana";
	tree += "djumic";
	tree += "jovan";
	tree += "sava";
	tree += "deni";
	tree += "nikola";

	return 0;
}
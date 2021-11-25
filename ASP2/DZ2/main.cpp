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

	tree += "luka";
	tree += "lazar";
	tree += "andjela";
	tree += "djordje";
	tree += "ilija";

	tree += "jovana";
	tree += "milica";
	tree += "zeljana";
	tree += "sanja";
	tree += "katarina";

	return 0;
}
#include "BStarTree.hpp"
#include <cmath>

using namespace std;

BStarTree::BStarTree(int degree) : degree(degree), maxKeys(degree - 1),
								   minNodeKeys(ceil((2 * degree - 1) / 3.0 - 1)), 
								   maxRootKeys(2 * floor((2 * degree - 2) / 3.0)) {}

BStarTree::~BStarTree() {

}
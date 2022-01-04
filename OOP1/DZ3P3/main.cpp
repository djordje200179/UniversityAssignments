#include "Road.hpp"
#include <iostream>

using namespace std;

int main() {
	Road road;
	road += {1, 2};
	road += {2, 3};
	road += {3, 0};

	cout << "Duzina: " << road.length() << endl;
	cout << road;

	return 0;
}
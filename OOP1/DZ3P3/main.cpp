#include "Road.hpp"
#include "Vehicle.hpp"
#include <iostream>

using namespace std;

int main() {
	try {
		Road road;
		road += {1, 2};
		road += {2, 3};
		road += {3, 0};

		cout << "Duzina: " << road.length() << endl;
		cout << road;
	} catch (const exception& e) {
		cout << e.what() << endl;
	}

	return 0;
}
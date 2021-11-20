#include <iostream>
#include "Skup.hpp"
#include "Rec.hpp"

using namespace std;

int main() {
	cout << std::boolalpha;

	Skup skup("ana voli milovana");

	cout << "n: " << skup('n') << endl;
	cout << "g: " << skup('g') << endl;
	skup += 'g';
	cout << "g: " << skup('g') << endl;

	Rec rec("medvedic");
	cout << rec << '=' << +rec << '/' << ~rec << endl;

	return 0;
}
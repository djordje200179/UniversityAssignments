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
	cout << "slog[0] = " << rec(0) << endl;
	cout << "slog[1] = " << rec(1) << endl;
	cout << "slog[2] = " << rec(2) << endl;
	cout << "slog[3] = " << rec(3) << endl;
	cout << "slog[-1] = " << rec(-1) << endl;
	cout << "slog[-2] = " << rec(-2) << endl;
	cout << "slog[-3] = " << rec(-3) << endl;

	return 0;
}
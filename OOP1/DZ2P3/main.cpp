#include <iostream>
#include "Skup.hpp"
#include "Rijec.hpp"

using namespace std;

int main() {
	cout << std::boolalpha;

	Skup skup("ana voli milovana");

	cout << "n: " << skup('n') << endl;
	cout << "g: " << skup('g') << endl;
	skup += 'g';
	cout << "g: " << skup('g') << endl;

	Rijec rijec("medvedic");
	cout << rijec << '=' << +rijec << '/' << ~rijec << endl;
	cout << "slog[0] = " << rijec(0) << endl;
	cout << "slog[1] = " << rijec(1) << endl;
	cout << "slog[2] = " << rijec(2) << endl;
	cout << "slog[3] = " << rijec(3) << endl;
	cout << "slog[-1] = " << rijec(-1) << endl;
	cout << "slog[-2] = " << rijec(-2) << endl;
	cout << "slog[-3] = " << rijec(-3) << endl;

	cout << "dugoprstic ^ prstic = " << (Rijec("dugoprstic") ^ Rijec("prstic")) << endl;
	cout << "slonic ^ prstic = " << (Rijec("slonic") ^ Rijec("prstic")) << endl;

	return 0;
}
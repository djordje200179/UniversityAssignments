#include <iostream>
#include "Skup.h"
#include "Rijec.h"

using namespace std;

void ispisiInfo(const Rijec& rijec) {
	cout << endl;
	cout << rijec << '=' << +rijec << '/' << ~rijec << endl;
	cout << "slog[0] = " << rijec(0) << endl;
	cout << "slog[1] = " << rijec(1) << endl;
	cout << "slog[2] = " << rijec(2) << endl;
	cout << "slog[3] = " << rijec(3) << endl;
	cout << "slog[-1] = " << rijec(-1) << endl;
	cout << "slog[-2] = " << rijec(-2) << endl;
	cout << "slog[-3] = " << rijec(-3) << endl;
	cout << endl;
}

int main() {
	cout << std::boolalpha;

	Skup skup("ana voli milovana");

	cout << "skup[n]: " << skup('n') << endl;
	cout << "skup[g]: " << skup('g') << endl;
	skup += 'g';
	cout << "skup[g]: " << skup('g') << endl;

	cout << "dugoprstic ^ prstic = " << (Rijec("dugoprstic") ^ Rijec("prstic")) << endl;
	cout << "slonic ^ prstic = " << (Rijec("slonic") ^ Rijec("prstic")) << endl;

	Rijec rijec("medvedic");
	ispisiInfo(rijec);

	cout << "Unesi rijec" << endl;
	cin >> rijec;
	ispisiInfo(rijec);

	cout << "Unesi rijec" << endl;
	cin >> rijec;
	ispisiInfo(rijec);

	return 0;
}
#include <iostream>
#include "Cvijet.h"
#include "Buket.h"
#include "Cvjecara.h"

using namespace std;

int main() {
	cout << boolalpha;

	const Cvijet ruza("Ruza", 15, 20);
	const Cvijet lala("Lala", 25, 43);

	Buket buket1;
	buket1.dodajCvijet(ruza);
	buket1.dodajCvijet(lala);
	cout << "buket1: " << buket1 << endl;
	buket1.dodajCvijet(ruza);
	cout << "buket1: " << buket1 << endl;

	const Buket buket2(buket1);
	buket1.dodajCvijet(lala);

	cout << "buket1: " << buket1 << endl;
	cout << "buket2: " << buket2 << endl;

	Buket buket3;
	buket3 = buket2;
	buket3.dodajCvijet(lala);
	cout << "buket2: " << buket2 << endl;
	cout << "buket3: " << buket3 << endl;

	return 0;
}
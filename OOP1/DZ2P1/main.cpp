#include <iostream>
#include "Cvet.hpp"
#include "Buket.hpp"
#include "Cvecara.hpp"

using namespace std;

int main() {
	Cvet ruza("Ruza", 10, 20);
	Cvet karanfil("Karanfil", 25, 35);
	Cvet lala("Lala", 5, 10);

	Buket buket1;
	buket1.dodajCvet(ruza);
	buket1.dodajCvet(lala);

	Buket buket2;
	buket2.dodajCvet(karanfil);
	buket2.dodajCvet(lala);
	buket2.dodajCvet(karanfil);
	buket2.dodajCvet(ruza);

	Cvecara cvecara;
	cvecara.dodajBuket(buket1);
	cvecara.dodajBuket(buket2);

	cout << cvecara;

	return 0;
}
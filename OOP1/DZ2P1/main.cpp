#include <iostream>
#include "Cvijet.hpp"
#include "Buket.hpp"
#include "Cvjecara.hpp"

using namespace std;

int main() {
	Cvijet ruza("Ruza", 10, 20);
	Cvijet karanfil("Karanfil", 25, 35);
	Cvijet lala("Lala", 5, 10);

	Buket buket1;
	buket1.dodajCvijet(ruza);
	buket1.dodajCvijet(lala);

	Buket buket2;
	buket2.dodajCvijet(karanfil);
	buket2.dodajCvijet(lala);
	buket2.dodajCvijet(karanfil);
	buket2.dodajCvijet(ruza);

	Cvjecara cvjecara;
	cvjecara.dodajBuket(buket1);
	cvjecara.dodajBuket(buket2);

	cout << cvjecara;

	return 0;
}
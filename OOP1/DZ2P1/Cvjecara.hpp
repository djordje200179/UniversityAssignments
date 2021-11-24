#ifndef CVJECARA_H
#define CVJECARA_H

#include "Buket.hpp"
#include <ostream>

class Cvjecara {
public:
	Cvjecara(int pocetnaZarada = 1000);
	Cvjecara(const Cvjecara& druga);
	Cvjecara(Cvjecara&& druga);
	Cvjecara& operator=(Cvjecara druga);
	~Cvjecara();

	void dodajBuket(const Buket& buket);
	void prodajBuket(int pozicija);

	friend std::ostream& operator<<(std::ostream& os, const Cvjecara& cvjecara);
private:
	friend void swap(Cvjecara& prva, Cvjecara& druga);

	struct Cvor {
		Buket buket;
		Cvor* sledeci;
	};

	int zarada;

	Cvor* pocetni = nullptr;
	Cvor* poslednji = nullptr;
};

#endif // CVJECARA_H
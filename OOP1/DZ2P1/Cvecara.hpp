#ifndef CVECARA_H
#define CVECARA_H

#include "Buket.hpp"
#include <ostream>

class Cvecara {
public:
	Cvecara(int pocetnaZarada = 1000);
	Cvecara(const Cvecara& druga);
	Cvecara(Cvecara&& druga);
	Cvecara& operator=(Cvecara druga);
	~Cvecara();

	void dodajBuket(const Buket& buket);
	void prodajBuket(int pozicija);

	friend std::ostream& operator<<(std::ostream& os, const Cvecara& cvecara);
private:
	friend void swap(Cvecara& prva, Cvecara& druga);

	struct Cvor {
		Buket buket;
		Cvor* sledeci;
	};

	int zarada;

	Cvor* pocetni = nullptr;
	Cvor* poslednji = nullptr;
};

#endif // CVECARA_H
#ifndef BUKET_H
#define BUKET_H

#include "Cvet.hpp"
#include <ostream>

class Buket {
public:
	Buket() = default;
	Buket(const Buket& drugi);
	Buket(Buket&& drugi);
	Buket& operator=(Buket drugi);
	~Buket();

	void dodajCvet(const Cvet& cvet);

	int izracunajNabavnuCenu() const;
	int izracunajProdajnuCenu() const;
	int izracunajZaradu() const;

	friend bool operator>(const Buket& pocetni, const Buket& drugi);
	friend std::ostream& operator<<(std::ostream& os, const Buket& buket);
private:
	friend void swap(Buket& pocetni, Buket& drugi);

	struct Cvor {
		Cvet cvet;
		Cvor* sledeci;
	};

	Cvor* pocetni = nullptr;
	Cvor* poslednji = nullptr;
};

#endif // BUKET_H
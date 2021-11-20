#ifndef CVET_H
#define CVET_H

#include <string>
#include <ostream>

class Cvet {
public:
	Cvet(std::string naziv, int nabavnaCena, int prodajnaCena);

	std::string dohvatiNaziv() const;
	int dohvatiNabavnuCenu() const;
	int dohvatiProdajuCenu() const;

	int izracunajZaradu() const;

	friend bool operator==(const Cvet& pocetni, const Cvet& drugi);
	friend std::ostream& operator<<(std::ostream& os, const Cvet& cvet);
private:
	std::string naziv;
	int nabavnaCena;
	int prodajnaCena;
};

#endif // CVET_H
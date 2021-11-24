#ifndef CVIJET_H
#define CVIJET_H

#include <string>
#include <ostream>

class Cvijet {
public:
	Cvijet(std::string naziv, int nabavnaCijena, int prodajnaCijena);

	std::string dohvatiNaziv() const;
	int dohvatiNabavnuCijenu() const;
	int dohvatiProdajuCijenu() const;

	int izracunajZaradu() const;

	friend bool operator==(const Cvijet& pocetni, const Cvijet& drugi);
	friend std::ostream& operator<<(std::ostream& os, const Cvijet& cvijet);
private:
	std::string naziv;
	int nabavnaCijena;
	int prodajnaCijena;
};

#endif // CVIJET_H
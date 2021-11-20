#ifndef BIBLIOTEKA_H
#define BIBLIOTEKA_H

#include <string>
#include <ostream>
#include "Knjiga.hpp"

class Biblioteka {
public:
	Biblioteka(std::string naziv, int kapacitet);
	Biblioteka(const Biblioteka& druga);
	Biblioteka(Biblioteka&& druga);
	Biblioteka& operator=(Biblioteka druga);
	~Biblioteka();

	std::string dohvatiNaziv() const;
	int dohvatiBrojKnjiga() const;
	int dohvatiKapacitet() const;

	const Knjiga& dohvatiKnjigu(int id) const;

	Biblioteka& operator+=(const Knjiga& knjiga);
	friend std::ostream& operator<<(std::ostream& os, const Biblioteka& biblioteka);
private:
	friend void swap(Biblioteka& prva, Biblioteka& druga);

	std::string naziv;
	int brojKnjiga = 0;
	int kapacitet;

	Knjiga** niz;
};

#endif // BIBLIOTEKA_H
#ifndef AVION_H
#define AVION_H

#include "Pilot.hpp"
#include <string>

class Avion {
public:
	Avion(const std::string& naziv, int maksBrojPutnika);
	Avion(const Avion&) = delete;
	Avion(Avion&&) = default;

	void postaviKapetana(Pilot* noviKapetan);
	void postaviKopilota(Pilot* noviKopilot);

	std::string dohvatiNaziv() const;
	Pilot* dohvatiKapetana() const;
	Pilot* dohvatiKopilota() const;
	int dohvatiMaksBrojPutnika() const;

	void ispisi() const;
private:
	std::string naziv;
	Pilot* kapetan;
	Pilot* kopilot;
	int maksBrojPutnika;
};

#endif // AVION_H
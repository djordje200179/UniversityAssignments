#ifndef PILOT_H
#define PILOT_H

#include <string>

class Pilot {
public:
	Pilot(const std::string& ime, int satiLetenja, bool naLetu = false);
	Pilot(const Pilot&) = delete;
	Pilot(Pilot&&) = default;

	std::string dohvatiIme() const;
	int dohvatiSateLetenja() const;
	bool dohvatiNaLetu() const;

	void dodajSateLetenja(int dodatniSatiLetenja);
	void postaviNaLetu(bool status);

	void ispisi() const;
private:
	std::string ime;
	int satiLetenja;
	bool naLetu;
};

#endif // PILOT_H
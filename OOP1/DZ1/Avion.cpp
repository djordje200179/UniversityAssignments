#include "Avion.hpp"
#include <iostream>

Avion::Avion(const std::string& naziv, int maksBrojPutnika) : naziv(naziv), maksBrojPutnika(maksBrojPutnika) {}

void Avion::postaviKapetana(Pilot* noviKapetan) {
	if(noviKapetan->dohvatiSateLetenja() < 100)
		return;

	kapetan = noviKapetan;

	kapetan->postaviNaLetu(true);
}

void Avion::postaviKopilota(Pilot* noviKopilot) {
	kopilot = noviKopilot;

	kopilot->postaviNaLetu(true);
}

std::string Avion::dohvatiNaziv() const {
	return std::string();
}

Pilot* Avion::dohvatiKapetana() const {
	return nullptr;
}

Pilot* Avion::dohvatiKopilota() const {
	return nullptr;
}

int Avion::dohvatiMaksBrojPutnika() const {
	return 0;
}

void Avion::ispisi() const {
	std::cout << "AVION:" << naziv << '-' << maksBrojPutnika << std::endl;
}
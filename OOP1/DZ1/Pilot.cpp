#include "Pilot.hpp"
#include <iostream>

Pilot::Pilot(const std::string& ime, int satiLetenja, bool naLetu) : ime(ime), satiLetenja(satiLetenja), naLetu(naLetu) {}

std::string Pilot::dohvatiIme() const {
	return ime;
}

int Pilot::dohvatiSateLetenja() const {
	return satiLetenja;
}

bool Pilot::dohvatiNaLetu() const {
	return naLetu;
}

void Pilot::dodajSateLetenja(int dodatniSatiLetenja) {
	satiLetenja += dodatniSatiLetenja;
}

void Pilot::postaviNaLetu(bool status) {
	naLetu = status;
}

void Pilot::ispisi() const {
	std::cout << ime << '(' << satiLetenja << ')' << '-' << (naLetu ? 'L' : 'N') << std::endl;
}
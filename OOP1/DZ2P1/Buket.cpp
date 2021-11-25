#include "Buket.h"

using namespace std;

Buket::Buket(const Buket& drugi) {
	for (Cvor* trenutni = drugi.pocetni; trenutni; trenutni = trenutni->sledeci)
		dodajCvijet(trenutni->cvijet);
}

Buket::Buket(Buket&& drugi) {
	pocetni = exchange(drugi.pocetni, nullptr);
	poslednji = exchange(drugi.poslednji, nullptr);
}

Buket& Buket::operator=(Buket drugi) {
	swap(*this, drugi);

	return *this;
}

Buket::~Buket() {
	for (Cvor* trenutni = pocetni; trenutni;) 
		delete exchange(trenutni, trenutni->sledeci);
}

void Buket::dodajCvijet(const Cvijet& cvijet) {
	poslednji = (poslednji ? poslednji->sledeci : pocetni) = new Cvor{ cvijet };
}

int Buket::izracunajNabavnuCijenu() const {
	int ukupno = 0;
	for (Cvor* trenutni = pocetni; trenutni; trenutni = trenutni->sledeci)
		ukupno += trenutni->cvijet.dohvatiNabavnuCijenu();

	return ukupno;
}

int Buket::izracunajProdajnuCijenu() const {
	int ukupno = 0;
	for (Cvor* trenutni = pocetni; trenutni; trenutni = trenutni->sledeci)
		ukupno += trenutni->cvijet.dohvatiProdajuCijenu();

	return ukupno;
}

int Buket::izracunajZaradu() const {
	return izracunajProdajnuCijenu() - izracunajNabavnuCijenu();
}

bool operator>(const Buket& pocetni, const Buket& drugi) {
	return pocetni.izracunajProdajnuCijenu() - drugi.izracunajProdajnuCijenu();
}

ostream& operator<<(ostream& os, const Buket& buket) {
	os << '(';
	for (Buket::Cvor* trenutni = buket.pocetni; trenutni; trenutni = trenutni->sledeci) {
		bool validan = true;
		for(Buket::Cvor* privremeni = buket.pocetni; privremeni != trenutni; privremeni = privremeni->sledeci)
			if (trenutni->cvijet == privremeni->cvijet) {
				validan = false;
				break;
			}

		if (validan) {
			if (trenutni != buket.pocetni)
				os << ", ";

			os << trenutni->cvijet;
		}		
	}
	os << ") ";

	os << buket.izracunajProdajnuCijenu() << "RSD";

	return os;
}

void swap(Buket& pocetni, Buket& drugi) {
	swap(pocetni.pocetni, drugi.pocetni);
	swap(pocetni.poslednji, drugi.poslednji);
}
#include "Buket.hpp"

using namespace std;

Buket::Buket(const Buket& drugi) {
	for (Cvor* trenutni = drugi.pocetni; trenutni; trenutni = trenutni->sledeci)
		dodajCvet(trenutni->cvet);
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

void Buket::dodajCvet(const Cvet& cvet) {
	poslednji = (poslednji ? poslednji->sledeci : pocetni) = new Cvor{ cvet };
}

int Buket::izracunajNabavnuCenu() const {
	int ukupno = 0;
	for (Cvor* trenutni = pocetni; trenutni; trenutni = trenutni->sledeci)
		ukupno += trenutni->cvet.dohvatiNabavnuCenu();

	return ukupno;
}

int Buket::izracunajProdajnuCenu() const {
	int ukupno = 0;
	for (Cvor* trenutni = pocetni; trenutni; trenutni = trenutni->sledeci)
		ukupno += trenutni->cvet.dohvatiProdajuCenu();

	return ukupno;
}

int Buket::izracunajZaradu() const {
	return izracunajProdajnuCenu() - izracunajNabavnuCenu();
}

bool operator>(const Buket& pocetni, const Buket& drugi) {
	return pocetni.izracunajProdajnuCenu() - drugi.izracunajProdajnuCenu();
}

ostream& operator<<(ostream& os, const Buket& buket) {
	os << '(';
	for (Buket::Cvor* trenutni = buket.pocetni; trenutni; trenutni = trenutni->sledeci) {
		bool validan = true;
		for(Buket::Cvor* privremeni = buket.pocetni; privremeni != trenutni; privremeni = privremeni->sledeci)
			if (trenutni->cvet == privremeni->cvet) {
				validan = false;
				break;
			}

		if (validan) {
			if (trenutni != buket.pocetni)
				os << ", ";

			os << trenutni->cvet;
		}		
	}
	os << ") ";

	os << buket.izracunajProdajnuCenu() << "RSD";

	return os;
}

void swap(Buket& pocetni, Buket& drugi) {
	swap(pocetni.pocetni, drugi.pocetni);
	swap(pocetni.poslednji, drugi.poslednji);
}
#include "Cvecara.hpp"

using namespace std;

Cvecara::Cvecara(int pocetnaZarada) : zarada(pocetnaZarada) {}

Cvecara::Cvecara(const Cvecara& druga) {
    for (Cvor* trenutni = druga.pocetni; trenutni; trenutni = trenutni->sledeci)
        dodajBuket(trenutni->buket);

    zarada = druga.zarada;
}

Cvecara::Cvecara(Cvecara&& druga) {
    pocetni = exchange(druga.pocetni, nullptr);
    poslednji = exchange(druga.poslednji, nullptr);

    zarada = druga.zarada;
}

Cvecara& Cvecara::operator=(Cvecara druga) {
    swap(*this, druga);

    return *this;
}

Cvecara::~Cvecara() {
    for (Cvor* trenutni = pocetni; trenutni;) {
        Cvor* sledeci = trenutni->sledeci;
        delete trenutni;
        trenutni = sledeci;
    }
}

void Cvecara::dodajBuket(const Buket& buket) {
    if (100 * buket.izracunajZaradu() / buket.izracunajNabavnuCenu() < 20)
        return;

    Cvor* novi = new Cvor{ buket };

    Cvor* prethodni = nullptr;
    for (Cvor* trenutni = pocetni;
         trenutni && trenutni->buket.izracunajProdajnuCenu() < buket.izracunajProdajnuCenu();
         prethodni = exchange(trenutni, trenutni->sledeci));

    if (prethodni) {
        novi->sledeci = exchange(prethodni->sledeci, novi);

        if (!novi->sledeci)
            poslednji = novi;
    } else
        pocetni = poslednji = novi;

    zarada -= buket.izracunajNabavnuCenu();
}

void Cvecara::prodajBuket(int pozicija) {
    Cvor* prethodni = nullptr;
    Cvor* trenutni = pocetni;

    for (int i = 0; i < pozicija; i++)
        prethodni = exchange(trenutni, trenutni->sledeci);

    zarada += trenutni->buket.izracunajProdajnuCenu();

    (prethodni ? prethodni->sledeci : pocetni) = trenutni->sledeci;
    delete trenutni;
}

ostream& operator<<(ostream& os, const Cvecara& cvecara) {
    os << "zarada=" << cvecara.zarada << "RSD" << endl;
    for (Cvecara::Cvor* trenutni = cvecara.pocetni; trenutni; trenutni = trenutni->sledeci)
        os << trenutni->buket << endl;

    return os;
}

void swap(Cvecara& prva, Cvecara& druga) {
    swap(prva.pocetni, druga.pocetni);
    swap(prva.poslednji, druga.poslednji);
}
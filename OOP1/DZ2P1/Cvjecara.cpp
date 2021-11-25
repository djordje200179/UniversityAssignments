#include "Cvjecara.h"

using namespace std;

Cvjecara::Cvjecara(int pocetnaZarada) : zarada(pocetnaZarada) {}

Cvjecara::Cvjecara(const Cvjecara& druga) {
    for (Cvor* trenutni = druga.pocetni; trenutni; trenutni = trenutni->sledeci)
        dodajBuket(trenutni->buket);

    zarada = druga.zarada;
}

Cvjecara::Cvjecara(Cvjecara&& druga) {
    pocetni = exchange(druga.pocetni, nullptr);
    poslednji = exchange(druga.poslednji, nullptr);

    zarada = druga.zarada;
}

Cvjecara& Cvjecara::operator=(Cvjecara druga) {
    swap(*this, druga);

    return *this;
}

Cvjecara::~Cvjecara() {
    for (Cvor* trenutni = pocetni; trenutni;)
        delete exchange(trenutni, trenutni->sledeci);
}

void Cvjecara::dodajBuket(const Buket& buket) {
    if (100 * buket.izracunajZaradu() / buket.izracunajNabavnuCijenu() < 20)
        return;

    Cvor* novi = new Cvor{ buket };

    Cvor* prethodni = nullptr;
    for (Cvor* trenutni = pocetni;
         trenutni && trenutni->buket.izracunajProdajnuCijenu() < buket.izracunajProdajnuCijenu();
         prethodni = exchange(trenutni, trenutni->sledeci));

    if (prethodni) {
        novi->sledeci = exchange(prethodni->sledeci, novi);

        if (!novi->sledeci)
            poslednji = novi;
    } else
        pocetni = poslednji = novi;

    zarada -= buket.izracunajNabavnuCijenu();
}

void Cvjecara::prodajBuket(int pozicija) {
    Cvor* prethodni = nullptr;
    Cvor* trenutni = pocetni;

    for (int i = 0; i < pozicija; i++)
        prethodni = exchange(trenutni, trenutni->sledeci);

    zarada += trenutni->buket.izracunajProdajnuCijenu();

    (prethodni ? prethodni->sledeci : pocetni) = trenutni->sledeci;
    delete trenutni;
}

ostream& operator<<(ostream& os, const Cvjecara& cvjecara) {
    os << "zarada=" << cvjecara.zarada << "RSD" << endl;
    for (Cvjecara::Cvor* trenutni = cvjecara.pocetni; trenutni; trenutni = trenutni->sledeci)
        os << trenutni->buket << endl;

    return os;
}

void swap(Cvjecara& prva, Cvjecara& druga) {
    swap(prva.pocetni, druga.pocetni);
    swap(prva.poslednji, druga.poslednji);
}
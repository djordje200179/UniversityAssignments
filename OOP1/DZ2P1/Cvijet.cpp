#include "Cvijet.hpp"

using namespace std;

Cvijet::Cvijet(const string& naziv, int nabavnaCijena, int prodajnaCijena) 
    : naziv(naziv), nabavnaCijena(nabavnaCijena), prodajnaCijena(prodajnaCijena) {}

string Cvijet::dohvatiNaziv() const {
    return naziv;
}

int Cvijet::dohvatiNabavnuCijenu() const {
    return nabavnaCijena;
}

int Cvijet::dohvatiProdajuCijenu() const {
    return prodajnaCijena;
}

int Cvijet::izracunajZaradu() const {
    return prodajnaCijena - nabavnaCijena;
}

bool operator==(const Cvijet& pocetni, const Cvijet& drugi) {
    return pocetni.naziv == drugi.naziv;
}

ostream& operator<<(ostream& os, const Cvijet& cvijet) {
    os << cvijet.naziv;

    return os;
}
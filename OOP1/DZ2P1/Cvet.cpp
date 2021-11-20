#include "Cvet.hpp"

using namespace std;

Cvet::Cvet(string naziv, int nabavnaCena, int prodajnaCena) 
    : naziv(naziv), nabavnaCena(nabavnaCena), prodajnaCena(prodajnaCena) {}

string Cvet::dohvatiNaziv() const {
    return naziv;
}

int Cvet::dohvatiNabavnuCenu() const {
    return nabavnaCena;
}

int Cvet::dohvatiProdajuCenu() const {
    return prodajnaCena;
}

int Cvet::izracunajZaradu() const {
    return prodajnaCena - nabavnaCena;
}

bool operator==(const Cvet& pocetni, const Cvet& drugi) {
    return pocetni.naziv == drugi.naziv;
}

ostream& operator<<(ostream& os, const Cvet& cvet) {
    os << cvet.naziv;

    return os;
}
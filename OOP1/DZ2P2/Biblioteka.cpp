#include "Biblioteka.hpp"

using namespace std;

Biblioteka::Biblioteka(string naziv, int kapacitet) : naziv(naziv), kapacitet(kapacitet) {
    niz = new Knjiga * [kapacitet];
}

Biblioteka::Biblioteka(const Biblioteka& druga) : Biblioteka(druga.naziv, druga.kapacitet) {
    for (int i = 0; i < druga.brojKnjiga; i++)
        *this += *druga.niz[i];
}

Biblioteka::Biblioteka(Biblioteka&& druga) : naziv(druga.naziv), kapacitet(druga.kapacitet), brojKnjiga(druga.brojKnjiga) {
    niz = exchange(druga.niz, nullptr);
}

Biblioteka& Biblioteka::operator=(Biblioteka druga) {
    swap(*this, druga);

    return *this;
}

Biblioteka::~Biblioteka() {
    for (int i = 0; i < brojKnjiga; i++)
        delete niz[i];

    delete[] niz;
}

string Biblioteka::dohvatiNaziv() const {
    return naziv;
}

int Biblioteka::dohvatiBrojKnjiga() const {
    return brojKnjiga;
}

int Biblioteka::dohvatiKapacitet() const {
    return kapacitet;
}

const Knjiga* Biblioteka::dohvatiKnjigu(int id) const {
    for (int i = 0; i < brojKnjiga; i++)
        if (niz[i]->dohvatiId() == id)
            return niz[i];

    return nullptr;
}

Biblioteka& Biblioteka::operator+=(const Knjiga& knjiga) {
    niz[brojKnjiga++] = !knjiga;

    return *this;
}

void swap(Biblioteka& prva, Biblioteka& druga) {
    swap(prva.naziv, druga.naziv);
    swap(prva.brojKnjiga, druga.brojKnjiga);
    swap(prva.kapacitet, druga.kapacitet);
    swap(prva.niz, druga.niz);
}

ostream& operator<<(ostream& os, const Biblioteka& biblioteka) {
    os << "BIBLIOTEKA " << biblioteka.naziv << ' ' << biblioteka.brojKnjiga << '/' << biblioteka.kapacitet << endl;
    for (int i = 0; i < biblioteka.brojKnjiga; i++)
        os << *biblioteka.niz[i] << endl;

    return os;
}

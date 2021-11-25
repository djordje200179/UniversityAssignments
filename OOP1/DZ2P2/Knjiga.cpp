#include "Knjiga.hpp"

using namespace std;

Knjiga::Knjiga(const string& naziv, const string& autor) : naziv(naziv), autor(autor) {}

string Knjiga::dohvatiNaziv() const {
    return naziv;
}

string Knjiga::dohvatiAutora() const {
    return autor;
}

int Knjiga::dohvatiId() const {
    return id;
}

Knjiga* Knjiga::operator!() const {
    return new Knjiga(naziv, autor);
}

ostream& operator<<(ostream& os, const Knjiga& knjiga) {
    os << "KNJIGA " << knjiga.id << " : " << knjiga.naziv << " - " << knjiga.autor;

    return os;
}
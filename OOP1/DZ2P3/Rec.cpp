#include "Rec.hpp"
#include "Skup.hpp"
#include <cctype>

using namespace std;

Rec::Rec(string inicijalizator) : karakteri(inicijalizator) {}

size_t Rec::operator+() const {
	return karakteri.length();
}

int Rec::operator~() const {
	int brojac = 0;
	for (int i = 0; i < +*this; i++)
		brojac += jeNosilacSloga(i);

	return brojac;
}

int Rec::operator()(int n) const {
	int ukupno = ~*this;
	if (n < 0)
		n += ukupno;

	if (n < 0 || n >= ukupno)
		return -1;

	int i = 0;
	for (int brojac = 0; brojac <= n; i++)
		if (jeNosilacSloga(i))
			brojac++;

	return i - 1;
}

bool Rec::jeNosilacSloga(size_t indeks) const {
	static Skup samoglasnici("aeiouAEIOU");
	static Skup sonanti("lnrLNR");

	if (samoglasnici(karakteri[indeks]))
		return true;

	if (!sonanti(karakteri[indeks]))
		return false;
	
	if((indeks != 0 && samoglasnici(karakteri[indeks - 1])) ||
	   (indeks != +*this - 1 && samoglasnici(karakteri[indeks + 1])))
	   return false;

	return true;
}

bool operator^(const Rec& prva, const Rec& druga) {
	int brojPoklapanja = (~prva == 1 || ~druga == 1) ? 1 : 2;

	int itPrvi = prva(-brojPoklapanja);
	int itDrugi = druga(-brojPoklapanja);

	int duzinaPrvi = +prva;
	int duzinaDrugi = +druga;

	if (duzinaDrugi - itDrugi != duzinaPrvi - itPrvi)
		return false;

	for (; itPrvi < duzinaPrvi; itPrvi++, itDrugi++)
		if (prva.karakteri[itPrvi] != druga.karakteri[itDrugi])
			return false;

	return true;
}

istream& operator>>(istream& is, Rec& rec) {
	string privremeni;
	is >> privremeni;

	rec.karakteri = "";
	for (char karakter : privremeni)
		if (isalpha(karakter))
			rec.karakteri += karakter;

	return is;
}

ostream& operator<<(ostream& os, const Rec& rec) {
	os << rec.karakteri;

	return os;
}
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
	for (int i = 0; i < karakteri.length(); i++)
		brojac += jeNosilacSloga(i);

	return brojac;
}

int Rec::operator()(int n) const {
	int total = ~*this;
	if (n < 0)
		n += total;

	if (n < 0 || n >= total)
		return -1;

	int i = 0;
	for (int brojac = 0; i < karakteri.length() && brojac < n; i++)
		if (jeNosilacSloga(i))
			brojac++;

	return i;
}

bool Rec::jeNosilacSloga(size_t indeks) const {
	Skup samoglasnici("aeiouAEIOU");
	Skup sonanti("lnrLNR");

	if (samoglasnici(karakteri[indeks]))
		return true;

	if (!sonanti(karakteri[indeks]))
		return false;
	
	if((indeks != 0 && samoglasnici(karakteri[indeks - 1])) ||
	   (indeks != karakteri.length() - 1 && samoglasnici(karakteri[indeks + 1])))
	   return false;

	return true;
}

bool operator^(const Rec& prva, const Rec& druga) {
	return false;
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
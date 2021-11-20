#include "Rec.hpp"
#include <cctype>

using namespace std;

Rec::Rec(string inicijalizator) : karakteri(inicijalizator) {}

size_t Rec::operator+() const {
	return karakteri.length();
}

int Rec::operator~() const {
	int brojac = 0;
	for (int i = 0; i < karakteri.length(); i++)
		brojac += jeSlog(i);

	return brojac;
}

int Rec::operator()(int n) const {
	if (n > 0) {
		int i = 0;
		for (int brojac = 0; i < karakteri.length() && brojac < n; i++)
			if (jeSlog(i))
				brojac++;

		return i != karakteri.length() ? i : -1;
	} else {
		int i = karakteri.length();
		for (int brojac = n; i < karakteri.length() && brojac; i++)
			if (jeSlog(i))
				brojac++;

		return i;
	}
}

bool Rec::jeSlog(size_t indeks) const {
	if (jeSamoglasnik(karakteri[indeks]))
		return true;

	if (!jeSonant(karakteri[indeks]))
		return false;
	
	if((indeks != 0 && jeSamoglasnik(karakteri[indeks - 1])) ||
	   (indeks != karakteri.length() - 1 && jeSamoglasnik(karakteri[indeks + 1])))
	   return false;

	return true;
}

bool Rec::jeSamoglasnik(char karakter) {
	static const char samoglasnici[] = { 'a', 'e', 'i', 'o', 'u' };
	
	for (int i = 0; i < sizeof(samoglasnici) / sizeof(char); i++)
		if (karakter == samoglasnici[i])
			return true;

	return false;
}

bool Rec::jeSonant(char karakter) {
	static const char sonanti[] = { 'l', 'n', 'r' };

	for (int i = 0; i < sizeof(sonanti) / sizeof(char); i++)
		if (karakter == sonanti[i])
			return true;

	return false;
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
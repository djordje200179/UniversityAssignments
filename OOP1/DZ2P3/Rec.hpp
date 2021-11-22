#ifndef REC_H
#define REC_H

#include <string>
#include <istream>

class Rec {
public:
	Rec(std::string inicijalizator);

	size_t operator+() const;
	int operator~() const;
	int operator()(int n) const;
	friend bool operator^(const Rec& prva, const Rec& druga);
	friend std::istream& operator>>(std::istream& is, Rec& rec);
	friend std::ostream& operator<<(std::ostream& os, const Rec& rec);
private:
	bool jeNosilacSloga(size_t indeks) const;

	std::string karakteri;
};

#endif // REC_H
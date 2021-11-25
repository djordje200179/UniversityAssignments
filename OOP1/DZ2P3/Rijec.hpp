#ifndef RIJEC_H
#define RIJEC_H

#include <string>
#include <istream>

class Rijec {
public:
	Rijec(std::string inicijalizator);

	size_t operator+() const;
	int operator~() const;
	int operator()(int n) const;
	friend bool operator^(const Rijec& prva, const Rijec& druga);
	friend std::istream& operator>>(std::istream& is, Rijec& rijec);
	friend std::ostream& operator<<(std::ostream& os, const Rijec& rijec);
private:
	bool jeNosilacSloga(size_t indeks) const;
	void filtrirajRijec(std::string rijec);

	std::string karakteri;
};

#endif // RIJEC_H
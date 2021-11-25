#ifndef SKUP_H
#define SKUP_H

#include <string>

class Skup {
public:
	Skup(std::string inicijalizator);
	Skup(const Skup&) = delete;
	Skup& operator=(const Skup&) = delete;

	Skup& operator+=(char karakter);
	bool operator()(char karakter) const;
private:
	std::string karakteri;
};

#endif // SKUP_H
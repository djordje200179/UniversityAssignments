#ifndef KNJIGA_H
#define KNJIGA_H

#include <string>
#include <ostream>

class Knjiga {
public:
	Knjiga(std::string naziv, std::string autor);
	Knjiga(const Knjiga&) = delete;
	Knjiga& operator=(const Knjiga&) = delete;

	std::string dohvatiNaziv() const;
	std::string dohvatiAutora() const;
	int dohvatiId() const;

	Knjiga* operator!() const;

	friend std::ostream& operator<<(std::ostream& os, const Knjiga& knjiga);
private:
	std::string naziv;
	std::string autor;
	int id = globalId++;
	inline static int globalId = 1;
};

#endif // KNJIGA_H
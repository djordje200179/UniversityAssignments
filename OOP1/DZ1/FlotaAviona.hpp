#ifndef FLOTA_AVIONA_H
#define FLOTA_AVIONA_H

#include <string>
#include "Avion.hpp"

class FlotaAviona {
public:
	FlotaAviona(const std::string& naziv);
	FlotaAviona(const FlotaAviona& toCopy);
	FlotaAviona(FlotaAviona&& toMove);
	~FlotaAviona();

	void dodajAvion(Avion* data);

	int dohvatiBrojAviona() const;
	int dohvatiMaksBrojPutnika() const;
	Avion* dohvatiNajveciAvion() const;

	void ispisi() const;
private:
	struct Node {
		Avion* data;
		Node* next = nullptr;
	};

	std::string naziv;
	Node* firstNode = nullptr;
	Node* lastNode = nullptr;
};

#endif // FLOTA_AVIONA_H
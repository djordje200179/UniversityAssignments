#include "FlotaAviona.hpp"
#include <iostream>

FlotaAviona::FlotaAviona(const std::string& naziv) : naziv(naziv) {}

FlotaAviona::FlotaAviona(const FlotaAviona& toCopy) : naziv(naziv) {
	for(auto currNode = toCopy.firstNode; currNode;)
		dodajAvion(currNode->data);
}

FlotaAviona::FlotaAviona(FlotaAviona&& toMove) : naziv(toMove.naziv) {
	firstNode = std::exchange(toMove.firstNode, nullptr);
	lastNode = std::exchange(toMove.lastNode, nullptr);
}

FlotaAviona::~FlotaAviona() {
	for(auto currNode = firstNode; currNode;) {
		auto nextNode = currNode->next;
		
		delete currNode;

		currNode = nextNode;
	}
}

void FlotaAviona::dodajAvion(Avion* data) {
	auto newNode = new Node { data };

	if(!lastNode)
		firstNode = newNode;
	else
		lastNode->next = newNode;

	lastNode = newNode;
}

int FlotaAviona::dohvatiBrojAviona() const {
	auto counter = 0;

	for(auto currNode = firstNode; currNode; currNode = currNode->next)
		counter++;

	return counter;
}

int FlotaAviona::dohvatiMaksBrojPutnika() const {
	auto numOfPassengers = 0;

	for(auto currNode = firstNode; currNode; currNode = currNode->next)
		numOfPassengers += currNode->data->dohvatiMaksBrojPutnika();

	return numOfPassengers;
}

Avion* FlotaAviona::dohvatiNajveciAvion() const {
	auto maxAvion = (Avion*)nullptr;

	for(auto currNode = firstNode; currNode; currNode = currNode->next) {
		auto data = currNode->data;

		if(!maxAvion || data->dohvatiMaksBrojPutnika() > maxAvion->dohvatiMaksBrojPutnika())
			maxAvion = data;
	}

	return maxAvion;
}

void FlotaAviona::ispisi() const {
	std::cout << naziv << std::endl;
	for(auto currNode = firstNode; currNode; currNode = currNode->next)
		currNode->data->ispisi();
}
#include "Pilot.hpp"
#include "Avion.hpp"
#include "FlotaAviona.hpp"
#include <memory>

int main() {
	auto jana = new Pilot("Jana Nisavic", 105);
	auto djumoje = new Pilot("Branislav Djumic", 115);
	auto djole = new Pilot("Djordje Milanovic", 85);
	auto crnogorac = new Pilot("Nikola Petric", 95);
	
	auto avion1 = new Avion("Avion 1", 150);
	avion1->postaviKapetana(jana);
	avion1->postaviKopilota(djole);

	auto avion2 = new Avion("Avion 2", 250);
	avion2->postaviKapetana(djumoje);
	avion2->postaviKopilota(djole);

	auto flota = new FlotaAviona("Vuciceve delije");
	flota->dodajAvion(avion1);
	flota->dodajAvion(avion2);

	jana->ispisi();
	djumoje->ispisi();
	djole->ispisi();
	crnogorac->ispisi();
	flota->ispisi();

	auto najveciAvion = flota->dohvatiNajveciAvion();
	najveciAvion->ispisi();

	for (auto ptr : { jana, djumoje, djole, crnogorac })
		delete ptr;

	for (auto ptr : { avion1, avion2 })
		delete ptr;

	delete flota;

	return 0;
}
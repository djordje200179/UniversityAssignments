#include "Team.hpp"
#include "Player.hpp"
#include "Match.hpp"
#include "Pair.hpp"
#include <iostream>

using namespace std;

int main() {
	Player players[] = {
		{ "Marko Markovic", 200 },
		{ "Janko Jovanovic" },
		{ "Djordje Milanovic", 500 },
		{ "Branislav Djumic", 1300 },
		{ "Jovan Nikolic" },
		{ "Aleksa Djuric", 850 },
		{ "Denijal Petric" },
		{ "Nikola Sendelj" },
		{ "Petar Markovic", 7500 }
	};

	Team team1("Sutjeska Foca", 5);
	team1.setPlayer(&players[0], 0);
	team1.setPlayer(&players[1], 1);
	team1.setPlayer(&players[2], 2);
	team1.setPlayer(&players[3], 4);

	Team team2("Rudar Pljevlja", 10);
	team2.setPlayer(&players[4], 0);
	team2.setPlayer(&players[5], 2);
	team2.setPlayer(&players[6], 4);
	team2.setPlayer(&players[7], 6);
	team2.setPlayer(&players[8], 8);

	Match match(&team1, &team2);
	match.play();

	auto result = match.getPoints();
	cout << "Ishod: " << result << endl;
	delete result.getFirst();
	delete result.getSecond();

	cout << match;

	return 0;
}
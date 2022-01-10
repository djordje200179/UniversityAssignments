#include "Team.hpp"
#include "Player.hpp"
#include "Match.hpp"
#include <iostream>

using namespace std;

int main() {
	try {
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
		PrivilegedTeam team2("Rudar Pljevlja", 750, 10);

		team1.setPlayer(&players[0], 0)
			.setPlayer(&players[1], 1)
			.setPlayer(&players[2], 2)
			.setPlayer(&players[3], 4);

		team2.setPlayer(&players[4], 0)
			.setPlayer(&players[5], 2)
			.setPlayer(&players[6], 4)
			.setPlayer(&players[7], 6)
			.setPlayer(&players[8], 8);

		Match match(team1, team2);
		match.play();

		cout << "Ishod: " << match.getPoints() << endl;
		cout << match;
	} catch (const exception& e) {
		cout << e.what() << endl;
	}

	return 0;
}
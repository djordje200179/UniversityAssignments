#include "Match.hpp"

using namespace std;

void Match::play() {
	auto& homeTeam = *teams.getFirst();
	auto& guestTeam = *teams.getSecond();

	double homeValue = homeTeam.teamValue();
	double guestValue = guestTeam.teamValue();

	if (homeValue > guestValue) {
		outcome = HOME_WIN;

		for (int i = 0; i < homeTeam.possiblePlayers(); i++)
			if (homeTeam[i])
				homeTeam[i]->changeValue(+10);

		for (int i = 0; i < guestTeam.possiblePlayers(); i++)
			if (guestTeam[i])
				guestTeam[i]->changeValue(-10);

	} else if (homeValue < guestValue) {
		outcome = GUEST_WIN;

		for (int i = 0; i < homeTeam.possiblePlayers(); i++)
			if (homeTeam[i])
				homeTeam[i]->changeValue(-10);

		for (int i = 0; i < guestTeam.possiblePlayers(); i++)
			if (guestTeam[i])
				guestTeam[i]->changeValue(+10);
	} else
		outcome = DRAW;

	played = true;
}

Pair<const int> Match::getPoints() const {
	if (!played)
		throw MatchNotPlayedException();

	switch (outcome) {
	case DRAW:
		pointsHome = 1, pointsGuest = 1;
		break;
	case HOME_WIN:
		pointsHome = 3, pointsGuest = 0;
		break;
	case GUEST_WIN:
		pointsHome = 0, pointsGuest = 3;
		break;
	}

	return Pair<const int>(&pointsHome, &pointsGuest);
}

ostream& operator<<(ostream& os, const Match& match) {
	os << match.teams << ' ';

	if (match.played) {
		switch (match.outcome) {
		case Match::HOME_WIN:
			os << "DOMACIN POBJEDIO";
			break;
		case Match::DRAW:
			os << "NERJESENO";
			break;
		case Match::GUEST_WIN:
			os << "GOST POBJEDIO";
			break;
		}
	}

	return os;
}
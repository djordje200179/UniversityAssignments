#include "Match.hpp"

using namespace std;

void Match::play() {
	auto& homeTeam = *teams.getFirst();
	auto& guestTeam = *teams.getSecond();

	int homeValue = homeTeam.teamValue();
	int guestValue = guestTeam.teamValue();

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
}

Pair<int> Match::getPoints() const {
	switch (outcome) {
	case DRAW:
		return Pair<int>(new int(1), new int(1));
	case HOME_WIN:
		return Pair<int>(new int(3), new int(0));
	case GUEST_WIN:
		return Pair<int>(new int(0), new int(3)); 
	case NOT_PLAYED:
	default:
		throw MatchNotPlayedException();
	}
}

ostream& operator<<(ostream& os, const Match& match) {
	os << match.teams << ' ';

	if (match.isPlayed())
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

	return os;
}
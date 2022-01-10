#ifndef MATCH_H
#define MATCH_H

#include "Pair.hpp"
#include "Team.hpp"
#include <ostream>
#include <exception>

class MatchNotPlayedException : public std::exception {
public:
	MatchNotPlayedException() : std::exception("Match isn't played") {}
};

class Match {
public:
	enum Outcome { 
		HOME_WIN, 
		DRAW, 
		GUEST_WIN 
	};

public:
	Match(Team& home, Team& guest) : teams(&home, &guest) {}

	void play();
	bool isPlayed() const { return played; }
	Pair<Team> getTeams() const { return teams; }
	Pair<const int> getPoints() const;

	friend std::ostream& operator<<(std::ostream& os, const Match& match);
private:
	bool played = false;
	mutable int pointsHome = 0, pointsGuest = 0;

	Pair<Team> teams;
	Outcome outcome;
};

#endif MATCH_H
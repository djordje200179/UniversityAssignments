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
		NOT_PLAYED,
		HOME_WIN, 
		DRAW, 
		GUEST_WIN 
	};

public:
	Match(Team* home, Team* guest) : teams(home, guest) {}

	void play();
	bool isPlayed() const { return outcome != NOT_PLAYED; }
	Pair<Team> getTeams() const { return teams; }
	Pair<int> getPoints() const;

	friend std::ostream& operator<<(std::ostream& os, const Match& match);
private:
	Pair<Team> teams;
	Outcome outcome = NOT_PLAYED;
};

#endif MATCH_H
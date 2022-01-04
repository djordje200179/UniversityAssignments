#ifndef TEAM_H
#define TEAM_H

#include <string>
#include "Player.hpp"
#include <ostream>
#include <exception>

class ValueTooLowException : public std::exception {
public:
	ValueTooLowException() : std::exception("Value of player too low to be added to the team") {}
};

class Team {
public:
	Team(const std::string& name, int capacity) : name(name), capacity(capacity) { init(); }
	Team(const Team& rhs) { copy(rhs); }
	Team(Team&& rhs) { move(rhs); }
	virtual ~Team() { destroy(); }
	Team& operator=(const Team& rhs);
	Team& operator=(Team&& rhs);

	int addedPlayers() const;
	int possiblePlayers() const { return capacity; }
	double teamValue() const;
	virtual void setPlayer(Player* player, int position) { players[position] = player; }

	Player* operator[](size_t index) const { return players[index]; }
	friend bool operator==(const Team& lhs, const Team& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Team& team) { team.print(os); return os; }
protected:
	virtual void print(std::ostream& os) const;
	void printPlayers(std::ostream& os) const;

	std::string name;
private:
	void init();
	void destroy() { delete[] players; }
	void copy(const Team& rhs);
	void move(Team& rhs);
	
	int capacity;
	Player** players;
};

class PrivilegedTeam : public Team {
public:
	PrivilegedTeam(const std::string& name, int minimalValue, int capacity)
		: Team(name, capacity), minimalValue(minimalValue) {}

	void setPlayer(Player* player, int position) override;
protected:
	void print(std::ostream& os) const override;
private:
	int minimalValue;
};

#endif // TEAM_H
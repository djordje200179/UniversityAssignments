#include "Team.hpp"
#include <utility>

using namespace std;

Team& Team::operator=(const Team& rhs) {
	if (this != &rhs) {
		destroy();
		copy(rhs);
	}

	return *this;
}

Team& Team::operator=(Team&& rhs) {
	if (this != &rhs) {
		destroy();
		move(rhs);
	}

	return *this;
}

int Team::addedPlayers() const {
	int counter = 0;

	for (int i = 0; i < capacity; i++)
		counter += players[i] != nullptr;

	return counter;
}

double Team::teamValue() const {
	int sum = 0;

	for (int i = 0; i < capacity; i++)
		if (players[i])
			sum += players[i]->getValue();

	return (double)sum / addedPlayers();
}

void Team::init() {
	players = new Player* [capacity];

	for (int i = 0; i < capacity; i++)
		players[i] = nullptr;
}

void Team::copy(const Team& rhs) {
	name = rhs.name;
	capacity = rhs.capacity;

	init();

	for (int i = 0; i < capacity; i++)
		players[i] = rhs.players[i];
}

void Team::move(Team& rhs) {
	name = std::move(rhs.name);
	capacity = rhs.capacity;
	players = exchange(rhs.players, nullptr);
}

bool operator==(const Team& lhs, const Team& rhs) {
	if (lhs.name != rhs.name || lhs.capacity != rhs.capacity)
		return false;

	for (int i = 0; i < lhs.capacity; i++) {
		if ((lhs.players[i] && !rhs.players[i]) || (!lhs.players[i] && rhs.players[i]))
			return false;

		if (!lhs.players[i] && !rhs.players[i])
			continue;

		if (!(*lhs.players[i] == *rhs.players[i]))
			return false;
	}

	return true;
}

void Team::printPlayers(ostream& os) const {
	for (int i = 0; i < capacity; i++) {
		if (i != 0)
			os << ',';

		if (players[i])
			os << *players[i];
		else
			os << "---";
	}
}

Team& Team::setPlayer(Player* player, int position) {
	players[position] = player;

	return *this;
}

void Team::print(ostream& os) const {
	os << name << '[';

	printPlayers(os);

	os << ']';
}

void PrivilegedTeam::print(ostream& os) const {
	os << name << '(' << minimalValue << ')' << '[';

	printPlayers(os);

	os << ']';
}

PrivilegedTeam& PrivilegedTeam::setPlayer(Player* player, int position) {
	if (player->getValue() < minimalValue)
		throw ValueTooLowException();
	else {
		Team::setPlayer(player, position);

		return *this;
	}
}
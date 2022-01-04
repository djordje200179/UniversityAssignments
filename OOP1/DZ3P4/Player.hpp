#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <ostream>

class Player {
public:
	Player(const std::string& name, int value = 1000) : name(name), value(value) {}

	int getValue() const { return value; }
	void changeValue(double percentage) { value += value * (percentage / 100); }

	friend bool operator==(const Player& lhs, const Player& rhs) {
		return lhs.name == rhs.name && lhs.value == rhs.value;
	}
	friend std::ostream& operator<<(std::ostream& os, const Player& player) {
		os << player.name << '#' << player.value;
		return os;
	}
private:
	std::string name;
	int value;
};

#endif // PLAYER_H
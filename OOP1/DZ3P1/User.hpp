#ifndef USER_H
#define USER_H

#include <string>
#include <ostream>

class User {
public:
	User(const std::string& name, const std::string& address) : name(name), address(address) {}
	User(const User& rhs) = delete;
	User& operator=(const User& rhs) = delete;

	friend std::ostream& operator<<(std::ostream& os, const User& user) {
		os << '(' << user.name << ')' << user.address;
		return os;
	}
private:
	std::string name;
	std::string address;
};

#endif // USER_H
#include "Email.hpp"

using namespace std;

ostream& operator<<(ostream& os, const Email& email) {
	email.print(os);
	return os;
}

void Email::print(ostream& os) const {
	os << title << '\n';
	os << sender << '\n';
	os << receiver;
}

void EmailWithText::setText(const string& text) {
	if (status == SENT)
		throw InvalidTextChangeException();
	else
		this->text = text;
}

void EmailWithText::print(ostream& os) const {
	Email::print(os);
	os << '\n';
	os << text;
}
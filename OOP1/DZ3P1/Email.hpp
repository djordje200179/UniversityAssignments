#ifndef EMAIL_H
#define EMAIL_H

#include "User.hpp"
#include <string>
#include <ostream>
#include <exception>

class InvalidTextChangeException : public std::exception {
public:
	InvalidTextChangeException() : std::exception("Can't change text after sending an email") {}
};

class Email {
public:
	enum Status { IN_PREPARATION, SENT, RECEIVED };
public:
	Email(const User& sender, const User& receiver, const std::string& title)
		: sender(sender), receiver(receiver), title(title) {}
	Email(const Email&) = default;
	Email(Email&&) = default;
	Email& operator=(const Email&) = default;
	Email& operator=(Email&&) = default;
	virtual ~Email() = default;

	const User& getSender() const { return sender; }
	const User& getReceiver() const { return receiver; }
	std::string getTitle() const { return title; }
	Status getStatus() const { return status; }

	virtual void send() = 0;
	virtual Email* copy() const = 0;

	friend std::ostream& operator<<(std::ostream& os, const Email& email);
protected:
	virtual void print(std::ostream& os) const;
	Status status = IN_PREPARATION;
private:
	const User& sender;
	const User& receiver;
	std::string title;
};

class EmailWithText : public Email {
public:
	using Email::Email;

	void send() override { status = SENT; }
	void setText(const std::string& text);
	EmailWithText* copy() const override { return new EmailWithText(*this); }
protected:
	void print(std::ostream& os) const override;
private:
	std::string text;
};
#endif // EMAIL_H
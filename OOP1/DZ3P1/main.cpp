#include "User.hpp"
#include "Email.hpp"
#include <iostream>

using namespace std;

int main() {
	User user1("Marko Markovic", "marko123@gmail.com");
	User user2("Jovan Jovanovic", "jovan@yahoo.com");

	EmailWithText email(user1, user2, "Test");
	email.setText("Ovo je proba");

	cout << email;

	return 0;
}
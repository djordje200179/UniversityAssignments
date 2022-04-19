#include "User.hpp"
#include "Email.hpp"
#include "Timestamp.hpp"
#include "List.hpp"
#include <iostream>

using namespace std;

int main() {
	try {
		List<int> l;

		User user1("Marko Markovic", "marko123@gmail.com");
		User user2("Jovan Jovanovic", "jovan@yahoo.com");

		EmailWithText email(user1, user2, "Test");
		email.setText("Ovo je proba");

		EmailWithText email2(email);
		email2 = email;

		cout << email;
	} catch (const exception& e) {
		cout << e.what() << endl;
	}

	return 0;
}
#include <iostream>

using namespace std;

#define LESS_THAN -1
#define IN_RANGE 0
#define BIGGER_THAN +1

int main() {
	while(true) {
		double lower, upper;

		cout << "Opseg: ";
		cin >> lower >> upper;

		double accuracy;
		cout << "Preciznost: ";
		cin >> accuracy;

		unsigned short counter = 0;
		while(true) {
			counter++;

			auto third = (upper - lower) / 3;
			auto mid1 = lower + third;
			auto mid2 = upper - third;

			double result1;
			cout << "Da li je vas broj " << mid1 << "? (-1/0/1) ";
			cin >> result1;

			if(result1 == IN_RANGE)
				break;
			else if(result1 == LESS_THAN) {
				upper = mid1 - 2 * accuracy;
				continue;
			}

			double result2;
			cout << "Da li je vas broj " << mid2 << "? (-1/0/1) ";
			cin >> result2;

			if(result2 == IN_RANGE)
				break;
			else if(result2 == LESS_THAN)
				lower = mid1 + 2 * accuracy, upper = mid2 - 2 * accuracy;
			else
				lower = mid2 + 2 * accuracy;
		}

		cout << "Broj pogodjen iz " << counter << " puta." << endl;

		bool choice;
		cout << "Da li zelite jos jedno pogadjanje?";
		cin >> choice;

		if(!choice)
			return 0;
	}
}
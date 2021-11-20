#include "Dictionary.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "sample.hpp"

using namespace std;

#ifndef EXAMPLE
auto predefinedWords = std::vector<std::pair<std::string, std::string>>();
#endif

int main() {
	Dictionary dictionary;

	while(true) {
		cout << "Odaberite opciju: " << endl;
		cout << "1) Ucitaj neki rijecnik" << endl;
		cout << "2) Trazi prevode" << endl;
		cout << "3) Dodaj rijec" << endl;
		cout << "4) Ispisi stablo" << endl;
		cout << "5) Izbrisi rijec" << endl;
		cout << "6) Trazi najkracu rijec sa prefiksom" << endl;
		cout << "7) Zavrsi program" << endl;

		int choice;
		cin >> choice;

		string temp1, temp2;
		switch(choice) {
		case 1:
			dictionary.addValues(predefinedWords);

			break;
		case 2:
			cout << "Unesi rijec: ";
			cin >> temp1;

			cout << "[";
			for(auto& translation : dictionary.getValues(temp1))
				cout << translation << ", ";
			cout << "\b\b" << "] " << std::endl;

			break;
		case 3:
			cout << "Unesi rijec: ";
			cin >> temp1;

			cout << "Unesi prevod: ";
			cin >> temp2;

			dictionary.addValue(temp1, temp2);

			break;
		case 4:
			dictionary.printTree();

			break;
		case 5:
			cout << "Unesi rijec: ";
			cin >> temp1;

			dictionary.deleteValues(temp1);

			break;
		case 6:
			cout << "Unesi prefiks: ";
			cin >> temp1;

			cout << dictionary.getSpecialKey(temp1) << std::endl;

			break;
		case 7:
			return 0;
		}
	}
}
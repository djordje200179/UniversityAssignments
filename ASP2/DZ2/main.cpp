#include <iostream>
#include "Tree.hpp"
#include <fstream>
#include <queue>

using namespace std;
using namespace BStarTree;

int main() {
	Tree* tree = nullptr;
	cout << boolalpha;

	while (true) {
		cout << "Odaberite opciju: " << endl;
		cout << "1) Kreiraj stablo" << endl;
		cout << "2) Unisti stablo" << endl;
		cout << "3) Inicijalizuj stablo" << endl;
		cout << "4) Ispisi stablo" << endl;
		cout << "5) Provjeri kljuc" << endl;
		cout << "6) Umetni kljuc" << endl;
		cout << "7) Obrisi kljuc" << endl;
		cout << "8) Pronadji k-ti element" << endl;
		cout << "9) Zavrsi program" << endl;

		int choice;
		cin >> choice;

		string tempS;
		int tempI;
		switch (choice) {
		case 1:
			cout << "Unesi stepen: ";
			cin >> tempI;

			delete tree;
			tree = new Tree(tempI);

			break;
		case 2:
			delete tree;
			tree = nullptr;

			break;
		case 3:
			cout << "Unesi ime datoteke: ";
			cin >> tempS;
			
			{
				ifstream file(tempS, ios_base::in);
				file >> *tree;
				file.close();
			}

			break;
		case 4:
			cout << *tree;

			break;
		case 5:
			cout << "Kljuc: ";
			cin >> tempS;

			cout << "Nalazi se: " << tree->keyExists(tempS) << endl;

			break;
		case 6:
			cout << "Kljuc: ";
			cin >> tempS;

			cout << "Dodan: " << tree->addKey(tempS) << endl;

			break;
		case 7:
			cout << "Kljuc: ";
			cin >> tempS;

			cout << "Izbrisan: " << tree->removeKey(tempS) << endl;

			break;
		case 8:
			cout << "k: ";
			cin >> tempI;

			cout << "k-ti kljuc: " << tree->findKthKey(tempI) << endl;

			break;
		case 9:
			delete tree;

			return 0;
		}
	}

	return 0;
}
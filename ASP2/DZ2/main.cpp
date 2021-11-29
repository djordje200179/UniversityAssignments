#include <iostream>
#include "BStarTree.hpp"
#include <fstream>
#include <queue>

using namespace std;

void testPointers(const BStarTree& tree) {
	queue<BStarTree::Node*> nodes;
	nodes.push(tree.root);

	while (!nodes.empty()) {
		auto curr = nodes.front();
		nodes.pop();

		for (auto child : curr->children) {
			if (!child)
				continue;

			nodes.push(child);

			if (child->parent != curr) {
				cout << "Parent mismatch on node \"" << child->keys[0] << '"' << endl;
				cout << "\treal parent is \"" << curr->keys[0] << '"' << endl;
				cout << "\twrong parent is \"" << (child->parent ? child->parent->keys[0] : "NULL") << '"' << endl;
				cout << endl;
			}
		}
	}

	cout << endl;
}

int main() {
	BStarTree* tree = nullptr;
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
			tree = new BStarTree(tempI);

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
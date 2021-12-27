#include <iostream>
#include <string>
#include "AddressFunction.hpp"
#include "StaticHashTable.hpp"
#include "DynamicHashTable.hpp"
#include "Student.hpp"
#include "CSVParser.hpp"

using namespace std;

#define CHECK(table) if(!table) { cout << "Tabela nije kreirana" << endl; break; }

int main() {
	HashTable* hashTable = nullptr;

	do {
		cout << "Odaberite opciju iz menija: " << '\n';
		cout << "1) Kreiranje tabele" << '\n';
		cout << "2) Unistavanje tabele" << '\n';
		cout << "3) Ucitavanje u tabelu iz datoteke" << '\n';
		cout << "4) Praznjenje tabele" << '\n';
		cout << "5) Informacije o tabeli" << '\n';
		cout << "6) Ispis tabele" << '\n';
		cout << "7) Pronalazak kljuca" << '\n';
		cout << "8) Ubacivanje kljuca" << '\n';
		cout << "9) Izbacivanje kljuca" << '\n';
		cout << "0) Kraj programa" << '\n';

		int choice;
		cin >> choice;

		switch (choice) {
		case 1:
		{
			delete exchange(hashTable, nullptr);
			
			cout << "Odaberite tip tabele" << '\n';
			cout << "1) Staticka tabela" << '\n';
			cout << "2) Dinamicka tabela" << '\n';

			cin >> choice;

			int bucketSize;
			cout << "Unesite velicinu baketa: ";
			cin >> bucketSize;

			int b;
			cout << "Unesite parametar hes funkcije: ";
			cin >> b;

			switch (choice) {
			case 1:
				int s1, s2;
				cout << "Unesite parametre adresne funkcije: ";
				cin >> s1 >> s2;

				hashTable = new StaticHashTable(bucketSize, b, SplitSequenceLinearHashing(s1, s2));

				break;
			case 2:
				int p;
				cout << "Unesite pocetnu velicinu tabele: ";
				cin >> p;

				hashTable = new DynamicHashTable(bucketSize, b, p);

				break;
			}

			break;
		}
		case 2:
		{
			delete exchange(hashTable, nullptr);

			break;
		}
		case 3:
		{
			CHECK(hashTable);

			string filePath;
			cout << "Unesite ime datoteke: ";
			cin >> filePath;

			CSVParser fileReader(filePath);

			size_t counter = 0;
			while (auto student = fileReader.readRow())
				counter += hashTable->insertKey(student->getId(), student);

			cout << "Uspjesno ubaceno " << counter << " kljuceva" << '\n';

			break;
		}
		case 4:
		{
			CHECK(hashTable);

			hashTable->clear();

			break;
		}
		case 5:
		{
			CHECK(hashTable);

			cout << "Broj umetnutih kljuceva: " << hashTable->keyCount() << '\n';
			cout << "Velicina tabele (broj baketa): " << hashTable->tableSize() << '\n';
			cout << "Popunjenost tabele: " << hashTable->fillRatio() << '\n';

			break;
		}
		case 6:
		{
			CHECK(hashTable);

			cout << *hashTable;

			break;
		}
		case 7:
		{
			CHECK(hashTable);

			unsigned int key;

			cout << "Kljuc: ";
			cin >> key;

			auto student = hashTable->findKey(key);

			if (!student) {
				cout << "Kljuc nije pronadjen" << '\n';
				break;
			}

			cout << "Odaberite akciju sa njim: " << '\n';
			cout << "1) Prijava ispita" << '\n';
			cout << "2) Odjava ispita" << '\n';

			cin >> choice;

			string exam;
			cout << "Ispit: ";
			cin >> exam;

			switch (choice) {
			case 1:
				student->addCourse(exam);
				break;
			case 2:
				student->removeCourse(exam);
				break;
			}

			break;
		}
		case 8:
		{
			CHECK(hashTable);

			string name;
			cout << "Unesite ime studenta: " << '\n';
			getline(cin, name);

			unsigned int key;
			cout << "Unesite indeks studenta: " << '\n';
			cin >> key;

			hashTable->insertKey(key, new Student(key, name));

			break;
		}
		case 9:
		{
			CHECK(hashTable);

			unsigned int key;

			cout << "Kljuc: ";
			cin >> key;

			auto result = hashTable->deleteKey(key, true);

			cout << (result ? "Kljuc uspjesno izbacen : Kljuc nije pronadjen") << '\n';

			break;
		}
		case 0:
		{
			delete hashTable;
			return 0;
		}
		}
	} while (true);
}
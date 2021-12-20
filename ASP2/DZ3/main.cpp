#include <iostream>
#include "AddressFunction.hpp"
#include "HashTable.hpp"
#include "Student.hpp"
#include "CSVParser.hpp"

using namespace std;

int main() {
	HashTable hashTable(3, 2, SplitSequenceLinearHashing(2, 3));
	CSVParser fileReader("students_10.csv");

	while (auto student = fileReader.readRow())
		hashTable.insertKey(student->getId(), student);

	hashTable.deleteKey(19960046, true);

	cout << hashTable;

	return 0;
}
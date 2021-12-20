#include <iostream>
#include "AddressFunction.hpp"
#include "HashTable.hpp"
#include "Student.hpp"
#include "CSVParser.hpp"

using namespace std;

int main() {
	HashTable hashTable(5, 10, SplitSequenceLinearHashing(2, 3));
	CSVParser fileReader("students_5000.csv");

	while (auto student = fileReader.readRow())
		hashTable.insertKey(student->getId(), student);

	cout << hashTable;

	return 0;
}
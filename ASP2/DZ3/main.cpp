#include <iostream>
#include "AddressFunction.hpp"
#include "DefaultHashTable.hpp"
#include "Student.hpp"
#include "CSVParser.hpp"

using namespace std;

void test(HashTable& hashTable) {
	CSVParser fileReader("students_500.csv");

	while (auto student = fileReader.readRow())
		hashTable.insertKey(student->getId(), student);

	cout << hashTable;
}

int main() {
	DefaultHashTable hashTable(10, 5, SplitSequenceLinearHashing(2, 3));
	test(hashTable);

	return 0;
}
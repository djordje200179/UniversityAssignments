#include <iostream>
#include "AddressFunction.hpp"
#include "StaticHashTable.hpp"
#include "DynamicHashTable.hpp"
#include "Student.hpp"
#include "CSVParser.hpp"

using namespace std;

void test(HashTable& hashTable) {
	CSVParser fileReader("students_10000.csv");

	size_t counter = 0;
	while (auto student = fileReader.readRow())
		counter += hashTable.insertKey(student->getId(), student);

	cout << "Keys successfully inserted: " << counter << endl;
	cout << "Keys in table: " << hashTable.keyCount() << endl;

	//cout << hashTable;
}

int main() {
	StaticHashTable staticHashTable(5, 15, SplitSequenceLinearHashing(3, 5));
	cout << "Testing static table:" << endl;
	test(staticHashTable);
	cout << endl;

	DynamicHashTable dynamicHashTable(5, 15, 11);
	cout << "Testing dynamic table:" << endl;
	test(dynamicHashTable);
	cout << endl;

	return 0;
}
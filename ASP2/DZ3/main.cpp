#include <iostream>
#include "AddressFunction.hpp"
#include "StaticHashTable.hpp"
#include "DynamicHashTable.hpp"
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
	StaticHashTable staticHashTable(10, 6, SplitSequenceLinearHashing(2, 3));
	test(staticHashTable);

	DynamicHashTable dynamicHashTable(10, 6, 4);
	test(dynamicHashTable);

	return 0;
}
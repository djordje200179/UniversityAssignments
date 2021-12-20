#pragma once

#include <ostream>
#include <vector>
#include "Student.hpp"
#include "AddressFunction.hpp"

class HashTable {
private:
	const int bucketSize, hashDegree;
	std::vector<std::vector<Student*>> table;
	const AddressFunction& addressFunction;

	unsigned int hashFunction(unsigned int key) const { return key % tableSize(); }
public:
	HashTable(int bucketSize, int hashDegree, const AddressFunction& addressFunction) :
		bucketSize(bucketSize), hashDegree(hashDegree), 
		addressFunction(addressFunction),
		table(1ull << hashDegree) {}

	Student* findKey(unsigned int key) const;
	bool insertKey(unsigned int key, Student* data);
	bool deleteKey(unsigned int key, bool callDestructor = false);

	void clear();

	int keyCount() const;
	int tableSize() const { return table.size(); }
	double fillRatio() const { return (double)keyCount() / (tableSize() * bucketSize); }

	friend std::ostream& operator<<(std::ostream& os, const HashTable& table);
};
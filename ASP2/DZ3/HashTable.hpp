#pragma once

#include <ostream>

class AddressFunction;
class Student;

class HashTable {
protected:
	const int bucketSize, hashDegree;

	unsigned int hashFunction(unsigned int key) const { return key % (1 << hashDegree); }

	virtual void print(std::ostream& os) const = 0;
public:
	HashTable(int bucketSize, int hashDegree) : bucketSize(bucketSize), hashDegree(hashDegree) {}
	virtual ~HashTable() = default;

	virtual Student* findKey(unsigned int key) const = 0;
	virtual bool insertKey(unsigned int key, Student* data) = 0;
	virtual bool deleteKey(unsigned int key, bool callDestructor = false) = 0;

	virtual void clear() = 0;
	virtual int keyCount() const = 0;

	friend std::ostream& operator<<(std::ostream& os, const HashTable& table) { table.print(os); return os; }
};
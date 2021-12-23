#include "DynamicHashTable.hpp"
#include "Student.hpp"

DynamicHashTable::DynamicHashTable(int bucketSize, int hashDegree, int rootBuckets) :
	HashTable(bucketSize, hashDegree),
	buckets(1 << rootBuckets) {
	for (auto& bucket : buckets)
		bucket = new Leaf;
}

void DynamicHashTable::print(std::ostream& os) const {
	//popraviti ispis
}

Student* DynamicHashTable::findKey(unsigned int key) const {
	return nullptr; //popraviti pronalazenje
}

bool DynamicHashTable::insertKey(unsigned int key, Student* data) {
	return false; //popraviti ubacivanje
}

bool DynamicHashTable::deleteKey(unsigned int key, bool callDestructor) {
	return false; //popraviti brisanje
}

void DynamicHashTable::clear() {
	for (auto& bucket : buckets)
		bucket = new Leaf; //popraviti brisanje
}

int DynamicHashTable::keyCount() const {
	return 0; //popraviti brojanje
}

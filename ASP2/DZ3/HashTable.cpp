#include "HashTable.hpp"
#include <algorithm>

using namespace std;

Student* HashTable::findKey(unsigned int key) const {
	unsigned int originalAddress = hashFunction(key), address = originalAddress;

	for (int attempt = 0; attempt < tableSize(); attempt++, address = addressFunction(key, originalAddress, attempt, tableSize())) {
		auto& bucket = table[address];

		auto it = find_if(bucket.begin(), bucket.end(), [key](Student* student) { return student->getId() == key; });

		if (it != bucket.end())
			return *it;

		if (bucket.size() != bucketSize)
			return nullptr;
	}

	return nullptr;
}

bool HashTable::insertKey(unsigned int key, Student* data) {
	if (findKey(key))
		return false;

	unsigned int originalAddress = hashFunction(key), address = originalAddress;

	for (int attempt = 0; attempt < tableSize(); attempt++, address = addressFunction(key, originalAddress, attempt, tableSize())) {
		auto& bucket = table[address];

		auto deletedIt = find(table[address].begin(), table[address].end(), nullptr);
		
		if (deletedIt != table[address].end()) {
			*deletedIt = data;
			return true;
		}

		if (bucket.size() != bucketSize) {
			bucket.push_back(data);
			return true;
		}
	}

	return false;
}

bool HashTable::deleteKey(unsigned int key, bool callDestructor) {
	unsigned int originalAddress = hashFunction(key), address = originalAddress;

	for (int attempt = 0; attempt < tableSize(); attempt++, address = addressFunction(key, originalAddress, attempt, tableSize())) {
		auto& bucket = table[address];

		auto it = find_if(bucket.begin(), bucket.end(), [key](Student* student) { return student->getId() == key; });
		
		if (it != bucket.end()) {
			if (callDestructor)
				delete* it;

			*it = nullptr;

			return true;
		}

		if (bucket.size() != bucketSize)
			return false;
	}

	return false;
}

void HashTable::clear() {
	for (auto& bucket : table)
		bucket.clear();
}

int HashTable::keyCount() const {
	int counter = 0;

	for (auto& bucket : table)
		for (auto data : bucket)
			if(data)
				counter++;

	return counter;
}

ostream& operator<<(ostream& os, const HashTable& table) {
	for (auto& bucket : table.table) {
		os << string(50, '-') << endl;

		if (bucket.size()) {
			for (auto& data : bucket) {
				os << "| ";

				if(data)
					os << *data;
				else
					os << "DELETED";

				os << endl;
			}
		} else
			os << "| EMPTY" << endl;
	}

	os << string(50, '-') << endl;

	return os;
}

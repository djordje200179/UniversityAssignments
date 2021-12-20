#include "HashTable.hpp"
#include <algorithm>

using namespace std;

Student* HashTable::findKey(unsigned int key) const {
	unsigned int address = hashFunction(key);
	int attempt = 0;

	while (true) {
		auto& bucket = table[address];

		auto full = bucket.size() == bucketSize;
		auto it = find_if(bucket.begin(), bucket.end(), [key](Student* student) { return student->getId() == key; });

		if (it != bucket.end()) {
			return *it;
		}

		if (!full)
			return nullptr;

		attempt++;
		if (attempt == tableSize())
			return nullptr;

		address = addressFunction(key, address, attempt, tableSize());
	}
}

bool HashTable::insertKey(unsigned int key, Student* data) {
	unsigned int address = hashFunction(key);
	int attempt = 0;

	while (table[address].size() == bucketSize) {
		auto deletedIt = find(table[address].begin(), table[address].end(), nullptr);
		auto hasDeleted = deletedIt == table[address].end();
		if (hasDeleted) {
			*deletedIt = data;
			return true;
		}

		attempt++;
		if (attempt == tableSize())
			return false;

		address = addressFunction(key, address, attempt, tableSize());
	}

	table[address].push_back(data);

	return true;
}

bool HashTable::deleteKey(unsigned int key, bool callDestructor) {
	unsigned int address = hashFunction(key);
	int attempt = 0;

	while (true) {
		auto& bucket = table[address];

		auto full = bucket.size() == bucketSize;
		auto it = find_if(bucket.begin(), bucket.end(), [key](Student* student) { return student->getId() == key; });

		if (it != bucket.end()) {
			if (callDestructor)
				delete *it;

			*it = nullptr;
			
			return true;
		}

		if (!full)
			return false;

		attempt++;
		if (attempt == tableSize())
			return false;

		address = addressFunction(key, address, attempt, tableSize());
	}
}

void HashTable::clear() {
	for (auto& bucket : table)
		bucket.clear();
}

int HashTable::keyCount() const {
	int counter = 0;

	for (auto& bucket : table)
		for (auto& data : bucket)
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

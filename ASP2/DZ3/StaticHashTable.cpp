#include "StaticHashTable.hpp"
#include "AddressFunction.hpp"
#include "Student.hpp"
#include <algorithm>

using namespace std;

StaticHashTable::StaticHashTable(int bucketSize, int hashDegree, const AddressFunction& addressFunction) :
	HashTable(bucketSize, hashDegree),
	addressFunction(addressFunction),
	table(1ull << hashDegree) {
	addressFunction.init(this);
}

void StaticHashTable::print(std::ostream& os) const {
	for (auto& bucket : table) {
		os << string(50, '-') << '\n';

		if (bucket.size()) {
			for (auto& data : bucket) {
				os << "| ";

				if (data)
					os << *data;
				else
					os << "DELETED";

				os << '\n';
			}
		} else
			os << "| EMPTY" << '\n';
	}

	os << string(50, '-') << '\n';
}

Student* StaticHashTable::findKey(unsigned int key) const {
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

bool StaticHashTable::insertKey(unsigned int key, Student* data) {
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

bool StaticHashTable::deleteKey(unsigned int key, bool callDestructor) {
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

void StaticHashTable::clear() {
	for (auto& bucket : table)
		bucket.clear();
}

size_t StaticHashTable::keyCount() const {
	size_t counter = 0;

	for (auto& bucket : table)
		for (auto data : bucket)
			if (data)
				counter++;

	return counter;
}
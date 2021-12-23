#include "AddressFunction.hpp"
#include "StaticHashTable.hpp"
#include "Student.hpp"

using namespace std;

void AddressFunction::init(StaticHashTable* hashTable) const {
	this->hashTable = hashTable;
}

unsigned int SplitSequenceLinearHashing::getAddress(unsigned int key, int address, int attempt, size_t size) const {
	auto toCompare = hashTable->table[address].front();

	auto movement = toCompare->getId() > key ? s1 : s2;

	return (address + attempt * movement) % size;
}
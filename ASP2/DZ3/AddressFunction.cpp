#include "AddressFunction.hpp"
#include "DefaultHashTable.hpp"
#include "Student.hpp"

using namespace std;

void AddressFunction::init(DefaultHashTable* hashTable) const {
	this->hashTable = hashTable;
}

unsigned int SplitSequenceLinearHashing::getAddress(unsigned int key, int address, int attempt, int size) const {
	auto toCompare = hashTable->table[address].front();

	auto movement = toCompare->getId() > key ? s1 : s2;

	return (address + attempt * movement) % size;
}
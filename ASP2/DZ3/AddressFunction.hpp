#pragma once

class HashTable;

class AddressFunction {
protected:
	mutable HashTable* hashTable;
public:
	void init(HashTable* hashTable) const;

	virtual unsigned int getAddress(unsigned int key, int address, int attempt, int size) const = 0;
	unsigned int operator()(unsigned int key, int address, int attempt, int size) const { return getAddress(key, address, attempt, size); }
};

class SplitSequenceLinearHashing : public AddressFunction {
private:
	int s1, s2;
public:
	SplitSequenceLinearHashing(int s1, int s2) : s1(s1), s2(s2) {}

	unsigned int getAddress(unsigned int key, int address, int attempt, int size) const override;
};
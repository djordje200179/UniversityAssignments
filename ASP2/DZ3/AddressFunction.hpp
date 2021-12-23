#pragma once

class StaticHashTable;

class AddressFunction {
protected:
	mutable StaticHashTable* hashTable = nullptr;
public:
	void init(StaticHashTable* hashTable) const;

	virtual unsigned int getAddress(unsigned int key, int address, int attempt, size_t size) const = 0;
	unsigned int operator()(unsigned int key, int address, int attempt, size_t size) const { return getAddress(key, address, attempt, size); }
};

class SplitSequenceLinearHashing : public AddressFunction {
private:
	int s1, s2;
public:
	SplitSequenceLinearHashing(int s1, int s2) : s1(s1), s2(s2) {}

	unsigned int getAddress(unsigned int key, int address, int attempt, size_t size) const override;
};
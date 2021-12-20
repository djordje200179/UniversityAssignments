#pragma once

class AddressFunction {
public:
	virtual unsigned int getAddress(unsigned int key, int address, int attempt, int size) const = 0;
	unsigned int operator()(unsigned int key, int address, int attempt, int size) const { 
		return getAddress(key, address, attempt, size); 
	}
};

class DoubleHashing : public AddressFunction {
private:
	int p, q;
public:
	DoubleHashing(int p, int q) : p(p), q(q) {}

	unsigned int getAddress(unsigned int key, int address, int attempt, int size) const override {
		return address + attempt * (q + key % p);
	}
};
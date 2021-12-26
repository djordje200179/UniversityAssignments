#pragma once

#include "HashTable.hpp"
#include <vector>

class AddressFunction;

class StaticHashTable : public HashTable {
private:
	std::vector<std::vector<Student*>> table;
	const AddressFunction& addressFunction;
protected:
	void print(std::ostream& os) const override;
public:
	StaticHashTable(int bucketSize, int hashDegree, const AddressFunction& addressFunction);
	~StaticHashTable() = default;

	Student* findKey(unsigned int key) const override;
	bool insertKey(unsigned int key, Student* data) override;
	bool deleteKey(unsigned int key, bool callDestructor = false) override;

	void clear() override;
	size_t keyCount() const override;
	size_t tableSize() const override { return table.size(); }

	friend class SplitSequenceLinearHashing;
};
#ifndef PAIR_H
#define PAIR_H

#include <ostream>

template<typename T>
class Pair {
public:
	Pair(T* first, T* second) : first(first), second(second) {}

	void setFirst(T* first) { this->first = first; }
	void setSecond(T* second) { this->second = second; }

	T* getFirst() const { return first; }
	T* getSecond() const { return second; }

	friend bool operator==(const Pair& lhs, const Pair& rhs) {
		return *lhs.first == *rhs.first && *lhs.second == *rhs.second;
	}
	friend std::ostream& operator<<(std::ostream& os, const Pair& pair) {
		os << '[' << *pair.first << '-' << *pair.second << ']';
		return os;
	}
private:
	T* first;
	T* second;
};
#endif // PAIR_H
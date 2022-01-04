#ifndef LISTA_H
#define LISTA_H

#include <utility>
#include <exception>

class InvalidIteratorException : public std::exception {
public:
	InvalidIteratorException() : std::exception("Current element doesn't exist") {}
};

template<typename T>
class List {
private:
	struct Node {
		T data;
		Node* next;
	};
public:
	List() = default;
	List(const List& rhs) { copy(rhs); }
	List(List&& rhs) { move(rhs); }
	~List() { destroy(); }
	List& operator=(const List& rhs) {
		if (this != &rhs) {
			destroy();
			copy(rhs);
		}

		return *this;
	}
	List& operator=(List&& rhs) {
		if (this != &rhs) {
			destroy();
			move(rhs);
		}

		return *this;
	}
	
	void append(const T& data) {
		end = (end ? end->next : start) = new Node{ data };
		elements++;
	}
	size_t length() const { return elements; }

	void resetIterator() const { iterator = start; }
	void moveIterator() const { iterator = iterator->next; }
	bool validIterator() const { return iterator != nullptr; }
	T& accessIterator() { 
		auto& constThis = const_cast<const List&>(*this);
		auto& constValue = constThis.accessIterator();
		auto& value = const_cast<T&>(constValue);
		return value;
	}
	const T& accessIteartor() const {
		if (!iterator)
			throw InvalidIteratorException();
		else
			return iterator->data;
	}
private:
	void destroy() {
		for (Node* curr = start; curr;)
			delete std::exchange(curr, curr->next);

		start = end = iterator = nullptr;
		elements = 0;
	}
	void copy(const List& rhs) {
		for (Node* curr = rhs.start; curr; curr = curr->next) {
			append(curr->data);

			if (curr = rhs.iterator)
				iterator = curr;
		}

		elements = rhs.elements;
	}
	void move(List& move) {
		start = std::exchange(move.start, nullptr);
		end = std::exchange(move.end, nullptr);
		iterator = std::exchange(move.iterator, nullptr);
		elements = std::exchange(rhs.length, 0);
	}

	Node* start = nullptr;
	Node* end = nullptr;
	size_t elements = 0;
	mutable Node* iterator = nullptr;
};
#endif // LISTA_H
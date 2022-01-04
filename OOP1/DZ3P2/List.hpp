#ifndef LIST_H
#define LIST_H

#include <utility>
#include <exception>

class IndexOutOfRangeException : public std::exception {
public:
	IndexOutOfRangeException() : std::exception("Index out of range") {}
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

	size_t length() const { return elements; }

	List& operator+=(const T& data) {
		end = (end ? end->next : start) = new Node{ data };
		elements++;

		return *this;
	}
	T& operator[](size_t index) {
		auto& constThis = const_cast<const List&>(*this);
		auto& constValue = constThis[index];
		auto& value = const_cast<T&>(constValue);
		return value;
	}
	const T& operator[](size_t index) const {
		Node* curr = start;

		for (size_t i = 0; i < index; i++) {
			curr = curr->next;

			if (!curr)
				throw IndexOutOfRangeException();
		}

		return curr->data;
	}
private:
	void destroy() {
		for (Node* curr = start; curr; )
			delete std::exchange(curr, curr->next);

		elements = 0;
	}
	void copy(const List& rhs) {
		for (Node* curr = rhs.start; curr; curr = curr->next)
			(*this) += curr->data;

		elements = rhs.elements;
	}
	void move(List& rhs) {
		start = std::exchange(rhs.start, nullptr);
		end = std::exchange(rhs.end, nullptr);
		elements = std::exchange(rhs.elements, 0);
	}

	Node* start = nullptr;
	Node* end = nullptr;
	size_t elements = 0;
};

#endif // LIST_H
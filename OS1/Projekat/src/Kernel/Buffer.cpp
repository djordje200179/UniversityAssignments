#include "../../h/Kernel/Buffer.hpp"

void Kernel::Buffer::put(char c) {
	count++;

	buffer[tail] = c;
	tail = (tail + 1) % BUFFER_SIZE;
}

char Kernel::Buffer::get() {
	count--;

	auto data = buffer[head];
	head = (head + 1) % BUFFER_SIZE;
	return data;
}
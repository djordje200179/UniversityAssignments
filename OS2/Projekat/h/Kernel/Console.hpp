#pragma once

#include "Semaphore.hpp"
#include "../syscall_c.h"

namespace Kernel {
class Console {
// Misc
private:
	class Buffer {
	public:
		static const int BUFFER_SIZE = 256;

		void put(char c);
		char get();

		bool empty() const { return count == 0; }
		bool full() const { return count == BUFFER_SIZE; }
	private:
		char buffer[BUFFER_SIZE];
		int head = 0, tail = 0, count = 0;
	};

// Nonstatic members
public:
	Console(const Console&) = delete;
	Console& operator=(const Console&) = delete;

	static Console& getInstance() {
		static Console instance;
		return instance;
	}
private:
	Console() = default;

public:
	char readChar() { return inputBuffer.get(); }
	void writeChar(char c);

	void waitForInputReady() const { sem_wait((sem_t)(inputItemAvailable)); }
	void waitForOutputReady() const { sem_wait((sem_t)(outputSpaceAvailable)); }

	void handleInterrupt() { tryRead(); tryWrite(); }
private:
	// Implement destructor

	void tryRead();
	void tryWrite();

	Semaphore* inputItemAvailable = new Semaphore(0);
	Semaphore* outputSpaceAvailable = new Semaphore(Buffer::BUFFER_SIZE);
	Buffer inputBuffer, outputBuffer;
};
}
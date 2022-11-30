#pragma once

#include "Buffer.hpp"
#include "Semaphore.hpp"
#include "../syscall_c.h"

namespace Kernel {
class Console {
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
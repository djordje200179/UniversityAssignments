#pragma once

#include "Singleton.hpp"
#include "Buffer.hpp"
#include "Semaphore.hpp"
#include "../syscall_c.h"

namespace Kernel {
SINGLETON_CLASS(Console)
public:
	char readChar() { return inputBuffer.get(); }
	void writeChar(char c);

	void waitForInputReady() const { sem_wait((sem_t)(inputItemAvailable)); }
	void waitForOutputReady() const { sem_wait((sem_t)(outputSpaceAvailable)); }

	void handleInterrupt() { tryRead(); tryWrite(); }
private:
	Console() = default;
	// Implement destructor

	void tryRead();
	void tryWrite();

	Semaphore* inputItemAvailable = new Semaphore(0);
	Semaphore* outputSpaceAvailable = new Semaphore(Buffer::BUFFER_SIZE);
	Buffer inputBuffer, outputBuffer;
};
}
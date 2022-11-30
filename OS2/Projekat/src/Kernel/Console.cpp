#include "../../h/Kernel/Console.hpp"

#define INPUT_READY (*(uint8*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT)
#define OUTPUT_READY (*(uint8*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT)

void Kernel::Console::writeChar(char c) {
	outputBuffer.put(c);
	handleInterrupt();
}

void Kernel::Console::tryRead() {
	while (INPUT_READY && !inputBuffer.full()) {
		inputBuffer.put(*(char*)CONSOLE_RX_DATA);
		inputItemAvailable->signal();
	}
}

void Kernel::Console::tryWrite() {
	while (OUTPUT_READY && !outputBuffer.empty()) {
		*(char*)CONSOLE_TX_DATA = outputBuffer.get();
		outputSpaceAvailable->signal();
	}
}
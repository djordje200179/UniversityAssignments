all: assembler linker emulator

assembler:
	@cd misc/assembler && make --no-print-directory

linker:
	@cd misc/linker && make --no-print-directory

emulator:
	@cd misc/emulator && make --no-print-directory

clean:
	@cd misc/common && make clean --no-print-directory
	@cd misc/assembler && make clean --no-print-directory
	@cd misc/linker && make clean --no-print-directory
	@cd misc/emulator && make clean --no-print-directory

.PHONY: assembler linker emulator clean
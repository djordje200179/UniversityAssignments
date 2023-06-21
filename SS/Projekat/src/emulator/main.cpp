#include <iostream>
#include <fstream>
#include "common/executable_file.hpp"
#include "emulator/context.hpp"
extern "C" {
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
}

termios init_console() {
	termios old_settings, new_settings;

	tcgetattr(STDIN_FILENO, &old_settings);

	new_settings = old_settings;
	new_settings.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	return old_settings;
}

void emulate(context& context);

int main(int argc, char** argv) {
	termios old_settings = init_console();

	try {
		executable_file executable_file;

		std::string input_file_path = argv[1];
		std::ifstream input_file(input_file_path, std::ios::binary);
		executable_file.deserialize(input_file);
		input_file.close();

		// std::cout << "Emulating " << input_file_path << std::endl;
		// std::cout << executable_file << std::endl;

		context context;
		context.memory = std::move(executable_file);

		emulate(context);

		std::cout << "Emulated processor executed halt instruction" << std::endl;
		std::cout << "Emulated processor state:" << std::endl;
		std::cout << context << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

	return 0;
}
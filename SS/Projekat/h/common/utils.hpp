#pragma once

#include <string>
#include <sstream>

std::string to_hex(uint32_t num);

class file_not_found_error : public std::exception {
private:
	std::string file_path;
	std::string error;

public:
	file_not_found_error(const std::string& file_path) {
		this->file_path = file_path;

		std::stringstream ss;
		ss << "File \"" << file_path << "\" not found.";
		error = ss.str();
	}

	const std::string& get_file_path() const { return file_path; }

	virtual const char* what() const noexcept override { return error.c_str(); }
};

class symbol_not_found_error : public std::exception {
private:
	std::string symbol_name;
	std::string error;

public:
	symbol_not_found_error(const std::string& symbol_name) {
		this->symbol_name = symbol_name;

		std::stringstream ss;
		ss << "Symbol \"" << symbol_name << "\" not found.";
		error = ss.str();
	}

	const std::string& get_symbol_name() const { return symbol_name; }

	virtual const char* what() const noexcept override {
		return error.c_str();
	}
};

class symbol_already_defined_error : public std::exception {
private:
	std::string symbol_name;
	std::string error;

public:
	symbol_already_defined_error(const std::string& symbol_name) {
		this->symbol_name = symbol_name;

		std::stringstream ss;
		ss << "Symbol \"" << symbol_name << "\" already defined.";
		error = ss.str();
	}

	const std::string& get_symbol_name() const { return symbol_name; }

	virtual const char* what() const noexcept override {
		return error.c_str();
	}
};
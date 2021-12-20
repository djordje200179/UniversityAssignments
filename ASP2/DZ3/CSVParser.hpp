#pragma once

#include <string>
#include <fstream>
#include "Student.hpp"

class CSVParser {
private:
	std::ifstream file;
public:
	CSVParser(const std::string& fileName);
	~CSVParser() { file.close(); }

	Student* readRow();
};
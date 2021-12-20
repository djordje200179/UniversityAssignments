#include "CSVParser.hpp"
#include <vector>
#include <sstream>

using namespace std;

CSVParser::CSVParser(const string& fileName) : file(fileName) {
	string line;
	getline(file, line);
}

Student* CSVParser::readRow() {
	string line;
	getline(file, line);

	if (line.empty())
		return nullptr;

	unsigned int studentId;
	string studentName;

	size_t commaLocation;
	commaLocation = line.find(",");
	studentId = stoi(line.substr(0, commaLocation));
	line.erase(0, commaLocation + 1);
	commaLocation = line.find(",");
	studentName = line.substr(0, commaLocation);
	line.erase(0, commaLocation + 1);

	auto student = new Student(studentId, studentName);

	istringstream ss(line);
	string token;
	while (ss >> token)
		student->addCourse(token);

	return student;
}
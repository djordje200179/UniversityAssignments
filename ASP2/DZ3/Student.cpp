#include "Student.hpp"
#include <iomanip>

using namespace std;

ostream& operator<<(ostream& os, const Student& student) {
	os << left << setw(20) << student.getName();
	os << ' ';
	os << '(' << student.getId() << ')';
	os << ':';

	os << ' ' << '[';
	for (auto& course : student.courses) {
		if (course != student.courses.front())
			os << ", ";

		os << course;
	}

	os << ']';

	return os;
}
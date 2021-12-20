#pragma once

#include <string>
#include <list>
#include <ostream>

class Student {
private:
	unsigned int id;
	std::string name;
	std::list<std::string> courses;

public:
	Student(unsigned int id, const std::string& name) : id(id), name(name) {}

	void addCourse(const std::string& course) { courses.push_back(course); }
	void removeCourse(const std::string& course) { courses.remove(course); }

	unsigned int getId() const { return id; }
	const std::string& getName() const { return name; }

	friend std::ostream& operator<<(std::ostream& os, const Student& student);
};
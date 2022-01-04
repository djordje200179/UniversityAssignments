#ifndef ROAD_H
#define ROAD_H

#include "List.hpp"
#include "Point.hpp"
#include <ostream>
#include <exception>

class PointAlreadyExistsException : public std::exception {
public:
	PointAlreadyExistsException() : std::exception("Point already exists") {}
};

class Road {
public:
	double length() const;

	Road& operator+=(Point point);
	friend std::ostream& operator<<(std::ostream& os, const Road& road);
private:
	List<Point> points;
};

#endif // ROAD_H
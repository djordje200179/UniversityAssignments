#include "Road.hpp"

using namespace std;

Road& Road::operator+=(Point point) {
	for (int i = 0; i < points.length(); i++)
		if (points[i] == point)
			throw PointAlreadyExistsException();

	points += point;

	return *this;
}

double Road::length() const {
	double length = 0.0;

	for (int i = 1; i < points.length(); i++)
		length += Point::distance(points[i], points[i - 1]);

	return length;
}

ostream& operator<<(ostream& os, const Road& road) {
	for (int i = 0; i < road.points.length(); i++) {
		if (i != 0)
			os << '\n';
		
		os << road.points[i];
	}

	return os;
}
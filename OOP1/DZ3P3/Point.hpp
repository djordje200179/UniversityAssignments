#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <ostream>

class Point {
public:
	Point(int x, int y) : x(x), y(y) {}

	static double distance(const Point& lhs, const Point& rhs) {
		return hypot(rhs.x - lhs.x, rhs.y - lhs.y);
	}

	friend bool operator==(const Point& lhs, const Point& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	friend std::ostream& operator<<(std::ostream& os, const Point& point) {
		os << '(' << point.x << ',' << point.y << ')';
		return os;
	}
private:
	int x, y;
};

#endif // POINT_H
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <ostream>

class Timestamp {
public:
	Timestamp(int year, int month, int day, int hour, int minute)
		: year(year), month(month), day(day), hour(hour), minute(minute) {}

	friend std::ostream& operator<<(std::ostream& os, const Timestamp& timestamp) {
		os << timestamp.day << '.' << timestamp.month << '.' << timestamp.year << '-' << timestamp.hour << ':' << timestamp.minute;
		return os;
	}
private:
	int year, month, day, hour, minute;
};

#endif // TIMESTAMP_H
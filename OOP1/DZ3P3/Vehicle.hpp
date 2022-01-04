#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ostream>
#include "Road.hpp"

class Vehicle {
public:
	Vehicle(const std::string& model) : model(model) {}

	double tripCost(const Road& road) const {
		return startCost() + road.length() * 0.1;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
		os << vehicle.model;
		return os;
	}
private:
	virtual double startCost() const = 0;

	std::string model;
};

class DefaultVehicle : public Vehicle {
public:
	using Vehicle::Vehicle;
private:
	double startCost() const override { return 120.0; }
};

#endif // VEHICLE_H
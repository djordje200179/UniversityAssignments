#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ostream>
#include "Road.hpp"

class Vehicle {
public:
	Vehicle(const std::string& model) : model(model) {}
	Vehicle(const Vehicle&) = default;
	Vehicle(Vehicle&&) = default;
	virtual Vehicle* copy() const = 0;
	Vehicle& operator=(const Vehicle&) = default;
	Vehicle& operator=(Vehicle&&) = default;
	virtual ~Vehicle() = default;

	double tripCost(const Road& road) const {
		return startCost() + road.length() * 0.1;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
		os << vehicle.model;
		return os;
	}
protected:
	virtual double startCost() const = 0;
private:
	std::string model;
};

class DefaultVehicle : public Vehicle {
public:
	using Vehicle::Vehicle;
	DefaultVehicle* copy() const override { return new DefaultVehicle(*this); }
protected:
	double startCost() const override { return 120.0; }
};

#endif // VEHICLE_H
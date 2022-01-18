#ifndef OPERATOR_H
#define OPERATOR_H

#include "Shipment.hpp"

class Operator {
public:
	Operator() = default;
	Operator(const Operator&) = default;
	Operator(Operator&&) = default;
	virtual Operator* copy() const = 0;
	Operator& operator=(const Operator&) = default;
	Operator& operator=(Operator&&) = default;
	virtual ~Operator() = default;
	virtual Operator* copy() const = 0;

	virtual void process(Shipment& shipment) const = 0;
protected:
	Shipment::Details& getDetails(Shipment& shipment) const { return shipment.details; }
};

#endif // OPERATOR_H
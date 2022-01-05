#ifndef OPERATOR_H
#define OPERATOR_H

class Shipment;

class Operator {
public:
	Operator() = default;
	Operator(const Operator&) = default;
	Operator(Operator&&) = default;
	Operator& operator=(const Operator&) = default;
	Operator& operator=(Operator&&) = default;
	virtual ~Operator() = default;

	virtual void process(Shipment& shipment) const = 0;
};

#endif // OPERATOR_H
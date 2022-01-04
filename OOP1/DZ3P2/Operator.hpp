#ifndef OPERATOR_H
#define OPERATOR_H

class Shipment;

class Operator {
public:
	virtual ~Operator() = default;

	virtual void process(Shipment& shipment) const = 0;
};

#endif // OPERATOR_H
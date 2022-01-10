#include "Shipment.hpp"
#include "Operator.hpp"

using namespace std;

int Shipment::idCounter = 0;

Shipment& Shipment::operator+=(const Operator& op) {
	if (detailsCalculated)
		throw AlreadyCalculatedException();
	else
		operators += op; 

	return *this;
}

Shipment::Details Shipment::getDetails() const {
	if (detailsCalculated)
		return details;
	else
		throw NotYetCalculatedException();
}

void Shipment::process() {
	for (int i = 0; i < operators.length(); i++)
		operators[i].process(*this);

	detailsCalculated = true;
}

ostream& operator<<(ostream& os, const Shipment& shipment) {
	os << "Posiljka[" << shipment.id << ", " << shipment.article.getName() << ']';
	return os;
}
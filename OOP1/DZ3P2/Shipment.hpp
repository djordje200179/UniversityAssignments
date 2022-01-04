#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "List.hpp"
#include "Operator.hpp"
#include "Article.hpp"
#include <ostream>
#include <exception>

class NotYetCalculatedException : public std::exception {
public:
	NotYetCalculatedException() : std::exception("Shipment not yet calculated") {}
};

class AlreadyCalculatedException : public std::exception {
public:
	AlreadyCalculatedException() : std::exception("Can't add operators after shipment is calculated") {}
};

class Shipment {
public:
	struct Details {
		int daysWaiting = 0;
		double shipmentCost = 0.0;
	};
public:
	Shipment(Article& article) : article(article) {}

	Shipment& operator+=(const Operator& op);
	friend std::ostream& operator<<(std::ostream& os, const Shipment& shipment);

	const Article& getArticle() const { return article; }
	int getId() const { return id; }
	Details getDetails() const;

	void process();
private:
	static int idCounter;

	int id = idCounter++;
	Article& article;
	List<const Operator&> operators;
	Details details;
	bool detailsCalculated = false;
};

#endif // SHIPMENT_H
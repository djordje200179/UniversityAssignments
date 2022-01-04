#ifndef SELLER_H
#define SELLER_H

#include "Operator.hpp"
#include "List.hpp"
#include "Article.hpp"
#include <string>

class Seller : public Operator {
public:
	struct Item {
		Article& article;
		double margin;
		int acquisitionDays;

		double sellingPrice() const { return article.getPurchasePrice() * margin; }
	};
public:
	Seller(const std::string& name) : name(name) {}
	
	std::string getName() const { return name; }

	void addItem(const Item& item) { items += item; }
	void addItem(Article& article, double margin, int acquisitionDays) { items += { article, margin, acquisitionDays}; }
	void process(Shipment& shipment) const override;
private:
	std::string name;
	List<Item> items;
};

#endif // SELLER_H
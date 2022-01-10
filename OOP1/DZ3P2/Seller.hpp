#ifndef SELLER_H
#define SELLER_H

#include "Operator.hpp"
#include "List.hpp"
#include "Article.hpp"
#include <string>

class Seller : public Operator {
private:
	struct Item {
		Article article;
		double margin;
		int acquisitionDays;

		double sellingPrice() const { return article.getPurchasePrice() * margin; }
	};
public:
	Seller(const std::string& name) : name(name) {}
	virtual Seller* copy() const override { return new Seller(*this); }
	
	std::string getName() const { return name; }

	Seller& addItem(const Article& article, double margin, int acquisitionDays) {
		items += { article, margin, acquisitionDays};
		return *this;
	}
	void process(Shipment& shipment) const override;
private:
	std::string name;
	List<Item> items;
};

#endif // SELLER_H
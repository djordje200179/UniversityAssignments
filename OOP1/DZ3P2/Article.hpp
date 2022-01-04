#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article {
public:
	Article(const std::string& name, int barcode, double purchasePrice)
		: name(name), barcode(barcode), purchasePrice(purchasePrice) {}

	std::string getName() const { return name; }
	int getBarcode() const { return barcode; }
	double getPurchasePrice() const { return purchasePrice; }

	friend bool operator==(const Article& lhs, const Article& rhs) {
		return lhs.barcode == rhs.barcode;
	}
private:
	std::string name;
	int barcode;
	double purchasePrice;
};

#endif // ARTICLE_H
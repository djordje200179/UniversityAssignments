#include "Seller.hpp"
#include "Shipment.hpp"
#include <iostream>

using namespace std;

int main() {
	try {
		Article book("Na Drini cuprija", 1365463, 12.5);

		Seller seller("Prodavac knjiga");
		seller.addItem(book, 1.1, 2);

		Shipment shipment(book);
		shipment += seller;

		shipment.process();

		auto details = shipment.getDetails();
		cout << shipment << endl;
		cout << details.daysWaiting << " - " << details.shipmentCost << endl;
	} catch (const exception& e) {
		cout << e.what() << endl;
	}

	return 0;
}
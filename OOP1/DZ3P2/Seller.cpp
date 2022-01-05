#include "Seller.hpp"
#include "Shipment.hpp"

void Seller::process(Shipment& shipment) const {
	auto& details = shipment.details;
	
	int n = items.length();
	for (int i = 0; i < n; i++) {
		auto& item = items[i];

		if (item.article == shipment.getArticle()) {
			details.daysWaiting += item.acquisitionDays;
			details.shipmentCost += item.sellingPrice();

			break;
		}
	}		
}
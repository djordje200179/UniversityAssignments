#include "Seller.hpp"
#include "Shipment.hpp"

void Seller::process(Shipment& shipment) const {
	auto& details = getDetails(shipment);

	for (int i = 0; i < items.length(); i++) {
		auto& item = items[i];

		if (item.article == shipment.getArticle()) {
			details.daysWaiting += item.acquisitionDays;
			details.shipmentCost += item.sellingPrice();

			break;
		}
	}		
}
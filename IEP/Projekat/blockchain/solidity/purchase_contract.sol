pragma solidity ^0.8.2;

contract Purchase {
	address payable courier = payable(address(0));
	address payable owner;
	address buyer;

	bool paid = false;
	bool delivered = false;

	modifier not_before_courier_joined() {
		require(courier != address(0), "Courier not joined.");
		_;
	}

	modifier only_buyer() {
		require(msg.sender == buyer, "Invalid customer account.");
		_;
	}

	modifier not_before_paying() {
		require(paid, "Transfer not complete.");
		_;
	}

	modifier not_after_paying() {
		require(!paid, "Transfer already complete.");
		_;
	}

	modifier not_after_delivered() {
        require(!delivered, "Already delivered.");
		_;
	}

	constructor (address _buyer) {
		owner = payable(msg.sender);
		buyer = _buyer;
	}

	function pay() external payable only_buyer not_after_paying {
		paid = true;
	}

	function join_courier(address payable _courier) external not_before_paying not_after_delivered {
		courier = _courier;
	}

	function delivery_finished() external not_before_courier_joined not_after_delivered {
		delivered = true;

		uint price = address(this).balance;
		owner.transfer(price * 80 / 100);
		courier.transfer(price * 20 / 100);
	}
}
import json
import sys

from eth_account import Account
from flask import Flask, request
from flask_jwt_extended import JWTManager, get_jwt
from web3.exceptions import ContractLogicError, InvalidAddress
from models import db, migrate, Category, Product, OrderProduct, Order
from config import Config
from common import check_permission, web3, purchase_contract, owner_account, purchase_contract_info

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)


@app.route("/search", methods=["GET"])
@check_permission("customer")
def search():
	category_name_pattern = request.args.get("category") or ""
	product_name_pattern = request.args.get("name") or ""

	categories = Category.query.filter(
		Category.name.like(f"%{category_name_pattern}%"),
		Category.products.any(Product.name.like(f"%{product_name_pattern}%"))
	).all()

	products = Product.query.filter(
		Product.name.like(f"%{product_name_pattern}%"),
		Product.categories.any(Category.name.like(f"%{category_name_pattern}%"))
	).all()

	return {
		"categories": [
			category.name
			for category
			in categories
		],
		"products": [
			{
				"id": product.id,
				"name": product.name,
				"price": product.price,
				"categories": [
					category.name
					for category
					in product.categories
				]
			}
			for product
			in products
		]
	}


@app.route("/order", methods=["POST"])
@check_permission("customer")
def order():
	buyer = get_jwt()["email"]

	if "requests" not in request.json:
		return {"message": "Field requests is missing."}, 400

	order_products = []
	total_price = 0
	for i, wanted_product in enumerate(request.json["requests"]):
		if "id" not in wanted_product:
			return {"message": f"Product id is missing for request number {i}."}, 400
		if "quantity" not in wanted_product:
			return {"message": f"Product quantity is missing for request number {i}."}, 400

		try:
			if int(wanted_product["id"]) <= 0:
				return {"message": f"Invalid product id for request number {i}."}, 400
		except ValueError:
			return {"message": f"Invalid product id for request number {i}."}, 400

		try:
			if int(wanted_product["quantity"]) <= 0:
				return {"message": f"Invalid product quantity for request number {i}."}, 400
		except ValueError:
			return {"message": f"Invalid product quantity for request number {i}."}, 400

		product = Product.query.get(wanted_product["id"])
		if product is None:
			return {"message": f"Invalid product for request number {i}."}, 400

		order_product = OrderProduct(
			product_id=product.id,
			quantity=wanted_product["quantity"],
			price=product.price
		)

		order_products.append(order_product)
		total_price += order_product.price * order_product.quantity

	if "address" not in request.json or request.json["address"] == "":
		return {"message": "Field address is missing."}, 400

	if not web3.is_address(request.json["address"]):
		return {"message": "Invalid address."}, 400

	try:
		contract_hash = purchase_contract.constructor(request.json["address"]).transact({
			"from": owner_account,
		})
		contract_receipt = web3.eth.wait_for_transaction_receipt(contract_hash)
	except InvalidAddress:
		return {"message": "Invalid address."}, 400

	order = Order(
		buyer=buyer,
		total_price=total_price,
		contract_address=contract_receipt.contractAddress
	)

	db.session.add(order)
	db.session.commit()

	for order_product in order_products:
		order_product.order_id = order.id
		db.session.add(order_product)

	db.session.commit()

	return {"id": order.id}


@app.route("/status", methods=["GET"])
@check_permission("customer")
def status():
	buyer = get_jwt()["email"]

	orders = Order.query.filter_by(buyer=buyer).all()

	return {
		"orders": [
			{
				"products": [
					{
						"categories": [
							category.name
							for category
							in order_product.product.categories
						],
						"name": order_product.product.name,
						"price": order_product.price,
						"quantity": order_product.quantity
					}
					for order_product
					in order.products
				],
				"price": order.total_price,
				"status": order.status,
				"timestamp": order.created_at
			}
			for order
			in orders
		]
	}


@app.route("/delivered", methods=["POST"])
@check_permission("customer")
def delivered():
	if "id" not in request.json:
		return {"message": "Missing order id."}, 400

	try:
		if int(request.json["id"]) <= 0:
			return {"message": "Invalid order id."}, 400
	except ValueError:
		return {"message": "Invalid order id."}, 400

	order = Order.query.get(request.json["id"])
	if order is None or order.status != "PENDING":
		return {"message": "Invalid order id."}, 400

	if "keys" not in request.json or request.json["keys"] == "":
		return {"message": "Missing keys."}, 400

	if "passphrase" not in request.json or request.json["passphrase"] == "":
		return {"message": "Missing passphrase."}, 400

	try:
		customer_keys = json.loads(request.json["keys"].replace("'", '"'))
		customer_private_key = Account.decrypt(customer_keys, request.json["passphrase"]).hex()
		customer_address = web3.to_checksum_address(customer_keys["address"])
	except ValueError:
		return {"message": "Invalid credentials."}, 400

	contract = web3.eth.contract(
		order.contract_address,
		abi=purchase_contract_info["abi"],
		bytecode=purchase_contract_info["bytecode"]
	)

	try:
		transaction = contract.functions.delivery_finished().build_transaction({
			"from": customer_address,
			"gasPrice": 21000,
			"nonce": web3.eth.get_transaction_count(customer_address)
		})

		signed_transaction = web3.eth.account.sign_transaction(transaction, customer_private_key)
		transaction_hash = web3.eth.send_raw_transaction(signed_transaction.rawTransaction)
		transaction_receipt = web3.eth.wait_for_transaction_receipt(transaction_hash)
	except ContractLogicError as err:
		reason = err.message[err.message.find("revert ") + 7:]

		return {"message": reason}, 400
	except ValueError:
		return {"message": "Invalid customer account."}, 400

	order.status = "COMPLETE"
	db.session.commit()

	return {}, 200


@app.route("/pay", methods=["POST"])
@check_permission("customer")
def pay():
	if "id" not in request.json:
		return {"message": "Missing order id."}, 400

	try:
		if int(request.json["id"]) <= 0:
			return {"message": "Invalid order id."}, 400
	except ValueError:
		return {"message": "Invalid order id."}, 400

	order = Order.query.get(request.json["id"])
	if order is None:
		return {"message": "Invalid order id."}, 400

	if "keys" not in request.json or request.json["keys"] == "":
		return {"message": "Missing keys."}, 400

	if "passphrase" not in request.json or len(request.json["passphrase"]) == 0:
		return {"message": "Missing passphrase."}, 400

	try:
		customer_keys = json.loads(request.json["keys"].replace("'", '"'))
		customer_private_key = Account.decrypt(customer_keys, request.json["passphrase"]).hex()
		customer_address = web3.to_checksum_address(customer_keys["address"])
	except ValueError:
		return {"message": "Invalid credentials."}, 400

	contract = web3.eth.contract(
		order.contract_address,
		abi=purchase_contract_info["abi"],
		bytecode=purchase_contract_info["bytecode"]
	)

	try:
		transaction = contract.functions.pay().build_transaction({
			"from": customer_address,
			"value": int(order.total_price),
			"gasPrice": 21000,
			"nonce": web3.eth.get_transaction_count(customer_address)
		})

		signed_transaction = web3.eth.account.sign_transaction(transaction, customer_private_key)
		transaction_hash = web3.eth.send_raw_transaction(signed_transaction.rawTransaction)
		transaction_receipt = web3.eth.wait_for_transaction_receipt(transaction_hash)
	except ContractLogicError as err:
		reason = err.message[err.message.find("revert ") + 7:]

		return {"message": reason}, 400
	except ValueError:
		return {"message": "Invalid customer account."}, 400

	return {}, 200


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

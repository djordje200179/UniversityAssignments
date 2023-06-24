import sys

from eth_account import Account
from flask import Flask, request
from flask_jwt_extended import JWTManager, get_jwt
from web3.exceptions import ContractLogicError

from models import db, migrate, Category, Product, OrderProduct, Order
from config import Config
from common import check_empty_fields, check_permission, web3, purchase_contract, owner_account, purchase_contract_info

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

	categories = Category.query.filter(Category.name.like(f"%{category_name_pattern}%")).all()
	products = Product.query.filter(Product.name.like(f"%{product_name_pattern}%")).all()

	return {
		"categories": [
			category.name
			for category
			in categories],
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
			in products]
	}


@app.route("/order", methods=["POST"])
@check_permission("customer")
@check_empty_fields(["requests"])
def order():
	buyer = get_jwt()["email"]
	wanted_products = request.json["requests"]
	buyer_address = request.json["address"]

	order_products = []
	total_price = 0
	for i, wanted_product in enumerate(wanted_products):
		if "id" not in wanted_product:
			return {"message": f"Product id is missing for request number {i}."}, 400
		elif "quantity" not in wanted_product:
			return {"message": f"Product quantity is missing for request number {i}."}, 400
		elif int(wanted_product["id"]) <= 0:
			return {"message": f"Invalid product id for request number {i}."}, 400
		elif int(wanted_product["quantity"]) <= 0:
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

	if not web3.is_address(buyer_address):
		return {"message": "Invalid address."}, 400

	contract_hash = purchase_contract.constructor(buyer_address).transact({
		"from": owner_account,
	})
	contract_receipt = web3.eth.wait_for_transaction_receipt(contract_hash)

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
@check_empty_fields(["keys", "passphrase"])
def delivered():
	if "id" not in request.json:
		return {"message": "Missing order id."}, 400

	order_id = request.json["id"]
	if int(order_id) <= 0:
		return {"message": "Invalid order id."}, 400

	order = Order.query.get(order_id)
	if order is None or order.status != "CREATED":
		return {"message": "Invalid order id."}, 400

	if order.status == "CREATED":
		return {"message": "Delivery not complete."}, 400

	if len(request.json["passphrase"]) == 0:
		return {"message": "Missing passphrase."}, 400

	try:
		customer_address = Account.decrypt(request.json["keys"], request.json["passphrase"]).hex()
	except ValueError:
		return {"message": "Invalid credentials."}, 400

	contract = web3.eth.contract(
		order.contract_ddress,
		abi=purchase_contract_info["abi"],
		bytecode=purchase_contract_info["bytecode"]
	)

	try:
		contract.functions.delivery_finished().transact({
			"from": customer_address
		})
	except ContractLogicError as err:
		reason = err.message[err.message.find("revert ") + 7:]

		return {"message": reason}, 400

	order.status = "DELIVERED"
	db.session.commit()

	return {}, 200


@app.route("/pay", methods=["POST"])
@check_permission("customer")
def pay():
	if "id" not in request.json:
		return {"message": "Missing order id."}, 400

	order_id = request.json["id"]
	if int(order_id) <= 0:
		return {"message": "Invalid order id."}, 400

	order = Order.query.get(order_id)
	if order is None:
		return {"message": "Invalid order id."}, 400

	if "keys" not in request.json:
		return {"message": "Missing keys."}, 400

	if "passphrase" not in request.json or len(request.json["passphrase"]) == 0:
		return {"message": "Missing passphrase."}, 400

	try:
		customer_address = Account.decrypt(request.json["keys"], request.json["passphrase"]).hex()
	except ValueError:
		return {"message": "Invalid credentials."}, 400

	contract = web3.eth.contract(
		order.contract_ddress,
		abi=purchase_contract_info["abi"],
		bytecode=purchase_contract_info["bytecode"]
	)

	try:
		contract.functions.pay().transact({
			"from": customer_address,
			"value": int(order.total_price)
		})
	except ContractLogicError as err:
		reason = err.message[err.message.find("revert ") + 7:]

		return {"message": reason}, 400


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

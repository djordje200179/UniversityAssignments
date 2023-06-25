from flask import Flask, request
from flask_jwt_extended import JWTManager
from web3.exceptions import ContractLogicError
from models import db, migrate, Order
from config import Config
from common import check_permission, web3, purchase_contract_info, owner_account

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)


@app.route("/orders_to_deliver", methods=["GET"])
@check_permission("courier")
def orders_to_deliver():
	orders = Order.query.filter(Order.status == "CREATED").all()

	return {
		"orders": [
			{
				"id": order.id,
				"email": order.buyer,
			}
			for order
			in orders
		]
	}


@app.route("/pick_up_order", methods=["POST"])
@check_permission("courier")
def pick_up_order():
	if "id" not in request.json:
		return {"message": "Missing order id."}, 400

	try:
		if int(request.json["id"]) <= 0:
			return {"message": "Invalid order id."}, 400
	except ValueError:
		return {"message": "Invalid order id."}, 400

	order = Order.query.get(request.json["id"])
	if order is None or order.status != "CREATED":
		return {"message": "Invalid order id."}, 400

	if "address" not in request.json or request.json["address"] == "":
		return {"message": "Missing address."}, 400

	if not web3.is_address(request.json["address"]):
		return {"message": "Invalid address."}, 400

	contract = web3.eth.contract(
		order.contract_address,
		abi=purchase_contract_info["abi"],
		bytecode=purchase_contract_info["bytecode"]
	)

	try:
		contract.functions.join_courier(request.json["address"]).transact({
			"from": owner_account,
		})
	except ContractLogicError as err:
		reason = err.message[err.message.find("revert ") + 7:]

		return {"message": reason}, 400

	order.status = "PENDING"
	db.session.commit()

	return {}, 200


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

from flask import Flask, request
from flask_jwt_extended import JWTManager
from web3.exceptions import ContractLogicError

from models import db, migrate, Order
from config import Config
from common import check_permission, check_empty_fields, web3, purchase_contract_info, owner_account

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)


@app.route("/orders_to_deliver", methods=["GET"])
@check_permission("courier")
def orders_to_deliver():
	orders = Order.query.all()

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
@check_empty_fields(["address"])
@check_permission("courier")
def pick_up_order():
	courier_address = request.json["address"]

	if "id" not in request.json:
		return {"message": "Missing order id."}, 400

	order_id = request.json["id"]
	if int(order_id) <= 0:
		return {"message": "Invalid order id."}, 400

	order = Order.query.get(order_id)
	if order is None or order.status != "CREATED":
		return {"message": "Invalid order id."}, 400

	if not web3.is_address(courier_address):
		return {"message": "Invalid address."}, 400

	contract = web3.eth.contract(
		order.contract_address,
		abi=purchase_contract_info["abi"],
		bytecode=purchase_contract_info["bytecode"]
	)

	try:
		contract.functions.join_courier(courier_address).transact({
			"from": owner_account,
		})
	except ContractLogicError as err:
		return {"message": str(err)}, 400

	order.status = "PENDING"
	db.session.commit()

	return {}, 200


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

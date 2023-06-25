import os
from flask import Flask, request
from flask_jwt_extended import JWTManager
from web3 import Web3, HTTPProvider
from models import db, migrate, Product, Category
from config import Config
from common import check_permission
import xmlrpc.client

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)
web3 = Web3(HTTPProvider(f"http://{os.environ['BLOCKCHAIN_HOST']}:8545"))
statistics_server = xmlrpc.client.ServerProxy(f"http://{os.environ['STATISTICS_HOST']}:8000")


@app.route("/update", methods=["POST"])
@check_permission("owner")
def update():
	if "file" not in request.files:
		return {"message": "Field file is missing."}, 400

	file_content = request.files["file"].stream.read().decode()
	lines = file_content.split('\n')

	for line_index, line in enumerate(lines):
		line_data = line.split(',')
		if len(line_data) != 3:
			return {"message": f"Incorrect number of values on line {line_index}."}, 400

	for line_index, line in enumerate(lines):
		category_names, name, price = line.split(',')

		try:
			if float(price) <= 0:
				return {"message": f"Incorrect price on line {line_index}."}, 400
		except ValueError:
			return {"message": f"Incorrect price on line {line_index}."}, 400

	new_products = []
	for line in lines:
		category_names, name, price = line.split(',')
		price = float(price)

		existing_product = Product.query.filter_by(name=name).first()
		if existing_product is not None:
			return {"message": f"Product {name} already exists."}, 400

		category_names = category_names.split('|')
		categories = []
		for category_name in category_names:
			category = Category.query.filter_by(name=category_name).first()
			if category is None:
				category = Category(name=category_name)
				db.session.add(category)
				db.session.commit()

			categories.append(category)

		new_product = Product(name=name, categories=categories, price=price)
		new_products.append(new_product)

	db.session.add_all(new_products)
	db.session.commit()

	return {}, 200


@app.route("/product_statistics", methods=["GET"])
@check_permission("owner")
def product_statistics():
	data = statistics_server.calculate_product_statistics()
	print(data, flush=True)
	return data


@app.route("/category_statistics", methods=["GET"])
@check_permission("owner")
def category_statistics():
	data = statistics_server.calculate_category_statistics()
	print(data, flush=True)
	return data


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

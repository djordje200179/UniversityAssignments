import os
from flask import Flask, request
from flask_jwt_extended import JWTManager
from web3 import Web3, HTTPProvider
from models import db, migrate, Product, Category
from config import Config
from common import check_permission

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)
web3 = Web3(HTTPProvider(f"http://{os.environ['BLOCKCHAIN_HOST']}:8545"))


@app.route("/update", methods=["POST"])
@check_permission("owner")
def update():
	if "file" not in request.files:
		return {"message": "Field file missing."}, 400

	file_content = request.files["file"].stream.read().decode()

	new_products = []
	for line in file_content.split('\n'):
		line_data = line.split(',')
		if len(line_data) != 3:
			return {"message": f"Incorrect number of values on line {len(new_products)}"}, 400

		if not line_data[2].isnumeric() and int(line_data[2]) < 0:
			return {"message": f"Incorrect price on line {len(new_products)}"}, 400

		existing_product = Product.query.filter_by(name=line_data[0]).first()
		if existing_product is not None:
			return {"message": f"Product {line_data[0]} already exists."}, 400

		categories = []
		category_names = line_data[0].split('|')
		for category_name in category_names:
			category = Category.query.filter_by(name=category_name).first()
			if category is None:
				category = Category(name=category_name)
				db.session.add(category)
				db.session.commit()

			categories.append(category)

		new_product = Product(name=line_data[1], categories=categories, price=int(line_data[2]))
		new_products.append(new_product)

	db.session.add_all(new_products)
	db.session.commit()

	return {}, 200


@app.route("/product_statistics", methods=["GET"])
@check_permission("owner")
def product_statistics():
	return "<p>Hello, World!</p>"


@app.route("/category_statistics", methods=["GET"])
@check_permission("owner")
def category_statistics():
	return "<p>Hello, World!</p>"


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

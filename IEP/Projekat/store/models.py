from flask_migrate import Migrate
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import func

db = SQLAlchemy()
migrate = Migrate()


class Category(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	name = db.Column(db.String(20), unique=True, nullable=False)

	products = db.relationship("Product", backref="category", lazy=True)


class Product(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	name = db.Column(db.String(40), unique=True, nullable=False)
	price = db.Column(db.Float, nullable=False)

	categories = db.relationship("Category", backref="products")


class Order(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	buyer = db.Column(db.Integer, nullable=False)
	total_price = db.Column(db.Float, nullable=False)
	status = db.Column(db.String(10), nullable=False, default="Pending")
	created_at = db.Column(db.DateTime, nullable=False, default=func.now())

	products = db.relationship("OrderProduct", backref="order")


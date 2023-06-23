from flask_migrate import Migrate
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import func

db = SQLAlchemy()
migrate = Migrate()

product_category = db.Table(
	"product_category",
	db.Column("product_id", db.ForeignKey("product.id"), primary_key=True),
	db.Column("category_id", db.ForeignKey("category.id"), primary_key=True)
)


class Category(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	name = db.Column(db.String(20), unique=True, nullable=False)

	products = db.relationship("Product", secondary=product_category, lazy=True)


class Product(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	name = db.Column(db.String(40), unique=True, nullable=False)
	price = db.Column(db.Integer, nullable=False)

	categories = db.relationship("Category", secondary=product_category)


class Order(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	buyer = db.Column(db.String(256), nullable=False)
	total_price = db.Column(db.Integer, nullable=False)
	status = db.Column(db.String(10), nullable=False, default="CREATED")
	created_at = db.Column(db.DateTime, nullable=False, default=func.now())
	contract_address = db.Column(db.String(42))

	products = db.relationship("OrderProduct")


class OrderProduct(db.Model):
	order_id = db.Column(db.Integer, db.ForeignKey("order.id"), primary_key=True)
	product_id = db.Column(db.Integer, db.ForeignKey("product.id"), primary_key=True)

	quantity = db.Column(db.Integer, nullable=False)
	price = db.Column(db.Integer, nullable=False)

	product = db.relationship("Product")
	order = db.relationship("Order", lazy=True)


from flask import Flask
from flask_jwt_extended import JWTManager
from models import db, migrate
from config import Config

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)


@app.route("/orders_to_deliver", methods=["GET"])
def orders_to_deliver():
	return "<p>Hello, World!</p>"


@app.route("/pick_up_order", methods=["GET"])
def pick_up_order():
	return "<p>Hello, World!</p>"


with app.app_context():
	db.create_all()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0", port=5003)
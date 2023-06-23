import re
from functools import wraps

from flask import Flask, request
from flask_jwt_extended import JWTManager, create_access_token, jwt_required, get_jwt_identity
from redis.client import Redis

from models import db, migrate, User
from config import Config

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
migrate.init_app(app, db)
jwt = JWTManager(app)


def check_empty_fields(fields):
	def decorator(function):
		@wraps(function)
		def wrapper(*args, **kwargs):
			for field in fields:
				if field not in request.json or request.json[field] == "":
					return {"message": f"Field {field} is missing."}, 400

			return function(*args, **kwargs)
		return wrapper
	return decorator


valid_email_regex = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,7}\b'


@check_empty_fields(["forename", "surname", "email", "password"])
def register_user(user_type):
	if len(request.json["email"]) > 256 or not re.match(valid_email_regex, request.json["email"]):
		return {"message": "Invalid email."}, 400

	if len(request.json["password"]) > 256 or len(request.json["password"]) < 8:
		return {"message": "Invalid password."}, 400

	existing_user = User.query.filter(User.email == request.json["email"]).first()
	if existing_user:
		return {"message": "Email already exists."}, 400

	new_user = User(
		email=request.json["email"],
		password=request.json["password"],
		forename=request.json["forename"],
		surname=request.json["surname"],
		user_type=user_type
	)

	db.session.add(new_user)
	db.session.commit()

	return {}, 200


@app.route("/register_customer", methods=["POST"])
def register_customer():
	return register_user("customer")


@app.route("/register_courier", methods=["POST"])
def register_courier():
	return register_user("courier")


@app.route("/login", methods=["POST"])
@check_empty_fields(["email", "password"])
def login():
	if len(request.json["email"]) > 256 or not re.match(valid_email_regex, request.json["email"]):
		return {"message": "Invalid email."}, 400

	user = User.query.filter(User.email == request.json["email"], User.password == request.json["password"]).first()
	if not user:
		return {"message": "Invalid credentials."}, 400

	claims = {
		"email": user.email,
		"user_type": user.user_type,
		"forename": user.forename,
		"surname": user.surname,
		"roles": [user.user_type]
	}

	access_token = create_access_token(identity=user.email, additional_claims=claims)

	return {"accessToken": access_token}, 200


@app.route("/delete", methods=["POST"])
@jwt_required()
def delete():
	email = get_jwt_identity()

	user = User.query.filter(User.email == email).first()
	if not user:
		return {"message": "Unknown user."}, 400

	db.session.delete(user)
	db.session.commit()

	with Redis(app.config["DELETED_USERS_DB_HOST"]) as deleted_users_db:
		deleted_users_db.setex(user.email, app.config["JWT_ACCESS_TOKEN_EXPIRES"], "deleted")

	return {}, 200


def create_owner():
	owner = User.query.filter(User.email == "onlymoney@gmail.com").first()
	if owner:
		return

	owner = User(
		email="onlymoney@gmail.com",
		password="evenmoremoney",
		forename="Scrooge",
		surname="McDuck",
		user_type="owner"
	)
	db.session.add(owner)
	db.session.commit()


with app.app_context():
	db.create_all()
	create_owner()

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0")

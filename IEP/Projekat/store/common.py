import os
from functools import wraps
from flask import request, current_app
from flask_jwt_extended import get_jwt, jwt_required
from redis.client import Redis
from web3 import Web3, HTTPProvider

web3 = Web3(HTTPProvider(f"http://{os.environ['BLOCKCHAIN_HOST']}:8545"))
owner_account = web3.eth.accounts[0]


def read_file(path):
	with open(path, 'r') as file:
		return file.read()


purchase_contract_info = {
	"bytecode": read_file("contracts/Purchase.bin"),
	"abi": read_file("contracts/Purchase.abi")
}

purchase_contract = web3.eth.contract(
	abi=purchase_contract_info["abi"],
	bytecode=purchase_contract_info["bytecode"]
)


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


def check_permission(user_type):
	def decorator(function):
		@wraps(function)
		@jwt_required()
		def wrapper(*args, **kwargs):
			claims = get_jwt()

			# with Redis(current_app.config["DELETED_USERS_DB_HOST"]) as deleted_users_db:
			# 	if deleted_users_db.exists(claims["email"]):
			# 		return {"message": "User was deleted."}, 403

			if claims["user_type"] != user_type:
				return {"msg": "Missing Authorization Header"}, 401

			return function(*args, **kwargs)
		return wrapper
	return decorator

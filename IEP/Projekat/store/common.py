from functools import wraps
from flask import request
from flask_jwt_extended import get_jwt


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
		def wrapper(*args, **kwargs):
			claims = get_jwt()

			if claims["user_type"] != user_type:
				return {"message": f"Only {user_type} can access this resource."}, 403

			return function(*args, **kwargs)
		return wrapper
	return decorator

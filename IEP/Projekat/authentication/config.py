import os
from datetime import timedelta


class Config:
	SQLALCHEMY_DATABASE_URI = f"mysql://{os.environ['DB_USERNAME']}:{os.environ['DB_PASSWORD']}@{os.environ['DB_HOST']}/users"
	JWT_SECRET_KEY = os.environ["JWT_KEY"]
	JWT_ACCESS_TOKEN_EXPIRES = timedelta(hours=1)

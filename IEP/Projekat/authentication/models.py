from flask_migrate import Migrate
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()
migrate = Migrate()


class User(db.Model):
	id = db.Column(db.Integer, primary_key=True)

	email = db.Column(db.String(256), unique=True, nullable=False)
	password = db.Column(db.String(256), nullable=False)
	forename = db.Column(db.String(256), nullable=False)
	surname = db.Column(db.String(256), nullable=False)
	user_type = db.Column(db.String(10), nullable=False)
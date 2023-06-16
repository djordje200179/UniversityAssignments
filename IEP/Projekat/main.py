from flask import Flask
from models import db

app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///project.db"

db.init_app(app)

@app.route("/")
def hello_world():
	return "<p>Hello, World!</p>"
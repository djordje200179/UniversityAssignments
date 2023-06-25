import json
import os
import subprocess
import xmlrpc.server


class Calculator:
	def calculate_product_statistics(self):
		os.environ["SPARK_APPLICATION_PYTHON_LOCATION"] = "/app/product_statistics.py"
		os.environ["SPARK_SUBMIT_ARGS"] = "--driver-class-path /app/mysql-connector-j-8.0.33.jar --jars /app/mysql-connector-j-8.0.33.jar"

		subprocess.run(["/template.sh"])

		with open("/app/results.json", "r") as file:
			results = [
				json.loads(row)
				for row in file
			]

		os.remove("/app/results.json")

		return {"statistics": results}

	def calculate_category_statistics(self):
		os.environ["SPARK_APPLICATION_PYTHON_LOCATION"] = "/app/category_statistics.py"
		os.environ["SPARK_SUBMIT_ARGS"] = "--driver-class-path /app/mysql-connector-j-8.0.33.jar --jars /app/mysql-connector-j-8.0.33.jar"

		subprocess.run(["/template.sh"])

		with open("/app/results.json", "r") as file:
			results = [row.strip() for row in file]

		os.remove("/app/results.json")

		return {"statistics": results}


server = xmlrpc.server.SimpleXMLRPCServer(("0.0.0.0", 8000))
server.register_instance(Calculator())
server.serve_forever()

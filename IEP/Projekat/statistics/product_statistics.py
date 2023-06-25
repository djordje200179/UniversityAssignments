import os
from pyspark.sql import SparkSession
from pyspark.sql.functions import col

builder = SparkSession.builder.appName("ProductStatistics")
builder = builder.master(f"spark://{os.environ['SPARK_HOST']}:7077")

spark = builder.config("spark.driver.extraClassPath", "mysql-connector-java-8.0.33.jar").getOrCreate()
spark.sparkContext.setLogLevel("ERROR")

product_df = spark.read.format("jdbc") \
	.option("driver", "com.mysql.cj.jdbc.Driver") \
	.option("url", f"jdbc:mysql://{os.environ['DB_HOST']}:3306/store") \
	.option("dbtable", "product") \
	.option("user", os.environ['DB_USERNAME']) \
	.option("password", os.environ['DB_PASSWORD']) \
	.load()

order_df = spark.read.format("jdbc") \
	.option("driver", "com.mysql.cj.jdbc.Driver") \
	.option("url", f"jdbc:mysql://{os.environ['DB_HOST']}:3306/store") \
	.option("dbtable", "order") \
	.option("user", os.environ['DB_USERNAME']) \
	.option("password", os.environ['DB_PASSWORD']) \
	.load()

order_product_df = spark.read.format("jdbc") \
	.option("driver", "com.mysql.cj.jdbc.Driver") \
	.option("url", f"jdbc:mysql://{os.environ['DB_HOST']}:3306/store") \
	.option("dbtable", "order_product") \
	.option("user", os.environ['DB_USERNAME']) \
	.option("password", os.environ['DB_PASSWORD']) \
	.load()

extended_order_product_df = product_df.alias("product") \
	.join(order_product_df.alias("order_product"), col("product.id") == col("order_product.product_id"), "inner") \
	.join(order_df.alias("order"), col("order_product.order_id") == col("order.id"), "inner")

sold_product_df = extended_order_product_df \
	.where(col("order.status") == "COMPLETED") \
	.groupBy("product.name") \
	.sum() \
	.reset_index() \
	.rename(columns={'sum': 'sold'})

waiting_product_df = extended_order_product_df \
	.where(col("order.status") != "COMPLETED") \
	.groupBy("product.name") \
	.sum() \
	.reset_index() \
	.rename(columns={'sum': 'waiting'})

product_info = sold_product_df \
	.join(waiting_product_df, "name", "outer") \
	.toJSON().collect()

print("Before writing to file", flush=True)

with open("/app/results.json", "w") as file:
	for row in product_info:
		file.write(row + '\n')

print("After writing to file", flush=True)

spark.stop()

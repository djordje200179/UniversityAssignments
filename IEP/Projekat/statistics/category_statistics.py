import os
from pyspark.sql import SparkSession
from pyspark.sql.functions import col

builder = SparkSession.builder.appName("CategoryStatistics")
builder = builder.master(f"spark://{os.environ['SPARK_HOST']}:7077")

spark = builder.config("spark.driver.extraClassPath", "mysql-connector-java-8.0.33.jar").getOrCreate()
spark.sparkContext.setLogLevel("ERROR")

category_df = spark.read.format("jdbc") \
	.option("driver", "com.mysql.cj.jdbc.Driver") \
	.option("url", f"jdbc:mysql://{os.environ['DB_HOST']}:3306/store") \
	.option("dbtable", "category") \
	.option("user", os.environ['DB_USERNAME']) \
	.option("password", os.environ['DB_PASSWORD']) \
	.load()

product_df = spark.read.format("jdbc") \
	.option("driver", "com.mysql.cj.jdbc.Driver") \
	.option("url", f"jdbc:mysql://{os.environ['DB_HOST']}:3306/store") \
	.option("dbtable", "product") \
	.option("user", os.environ['DB_USERNAME']) \
	.option("password", os.environ['DB_PASSWORD']) \
	.load()

product_category_df = spark.read.format("jdbc") \
	.option("driver", "com.mysql.cj.jdbc.Driver") \
	.option("url", f"jdbc:mysql://{os.environ['DB_HOST']}:3306/store") \
	.option("dbtable", "product_category") \
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
	.join(order_df.alias("order"), col("order_product.order_id") == col("order.id"), "inner") \
	.join(product_category_df.alias("product_category"), col("product.id") == col("product_category.product_id"), "inner") \
	.join(category_df.alias("category"), col("product_category.category_id") == col("category.id"), "inner")

sold_product_df = extended_order_product_df \
	.where(col("order.status") == "COMPLETED") \
	.groupBy("category.name") \
	.sum() \
	.reset_index() \
	.orderBy(col("sum").desc())\
	.orderBy(col("category.name").asc())\
	.select("category.name")

category_info = sold_product_df\
	.toJSON().collect()

print("Before writing to file", flush=True)

with open("/app/results.json", "w") as file:
	for row in category_info:
		file.write(row + '\n')

print("After writing to file", flush=True)

spark.stop()

import os
from pyspark.sql import SparkSession
from pyspark.sql.functions import col, sum, when, lit

builder = SparkSession.builder \
	.appName("CategoryStatistics") \
	.master(f"spark://{os.environ['SPARK_HOST']}:7077")

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
	.option("dbtable", "`order`") \
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

category_info = category_df.alias("category") \
	.join(product_category_df.alias("product_category"), col("category.id") == col("product_category.category_id"), "left") \
	.join(product_df.alias("product"), col("product_category.product_id") == col("product.id"), "left") \
	.join(order_product_df.alias("order_product"), col("product.id") == col("order_product.product_id"), "left") \
	.join(order_df.alias("order1"), col("order_product.order_id") == col("order1.id"), "left") \
	.groupBy("category.name") \
	.agg(sum(when(col("order1.id") == "COMPLETE", col("order_product.quantity")).otherwise(lit(0))).alias("sold")) \
	.orderBy(col("sold").desc(), col("category.name"))

category_names = [str(row["name"]) for row in category_info.collect()]
with open("/app/results.json", "w") as file:
	for row in category_names:
		file.write(row + '\n')

spark.stop()

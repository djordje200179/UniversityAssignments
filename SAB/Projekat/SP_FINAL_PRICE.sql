CREATE PROCEDURE SP_FINAL_PRICE(@orderId int, @currentDate datetime, @price decimal(10,3) out)
AS BEGIN
	IF (SELECT price FROM [Order] WHERE idOrder = @orderId) IS NULL BEGIN
		DECLARE @ordersCount INT
		SELECT @ordersCount = COUNT(*)
		FROM [Order] JOIN Buyer ON ([Order].idBuyer = Buyer.idBuyer)
		WHERE DATEDIFF(DAY, [Order].sentTime, @currentDate) < 30

		UPDATE [Order]
		SET price = (
			SELECT SUM(OrderArticle.quantity * Article.price * (100 - Shop.discount) / 100.0)
			FROM OrderArticle 
				JOIN Article ON (OrderArticle.idArticle = Article.idArticle) 
				JOIN Shop ON (Shop.idShop = Article.idShop)
			WHERE idOrder = @orderId
		) * IIF(@ordersCount > 0, 0.98, 1.00)
		WHERE idOrder = @orderId
	END

	SELECT @price = price
	FROM [Order]
	WHERE idOrder = @orderId
END
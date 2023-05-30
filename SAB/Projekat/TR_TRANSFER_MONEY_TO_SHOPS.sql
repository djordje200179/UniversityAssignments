CREATE TRIGGER TR_TRANSFER_MONEY_TO_SHOPS ON [Order] AFTER UPDATE AS BEGIN
	DECLARE orderCursor CURSOR FOR 
		SELECT idOrder, price, receivedTime, systemDiscount 
		FROM INSERTED
		WHERE state = 'arrived'

	DECLARE @orderId INT
	DECLARE @orderPrice DECIMAL(10,3)
	DECLARE @orderReceivedTime DATETIME
	DECLARE @systemDiscount BIT

	OPEN orderCursor
	FETCH NEXT FROM orderCursor INTO @orderId, @orderPrice, @orderReceivedTime, @systemDiscount

	WHILE @@FETCH_STATUS = 0 BEGIN
		DECLARE @shopAmountMultiplier DECIMAL(10,3)
		IF @systemDiscount = 1
			SET @shopAmountMultiplier = 0.97
		ELSE
			SET @shopAmountMultiplier = 0.95

		DECLARE shopCursor CURSOR FOR
			SELECT idShop, SUM(OrderArticle.price*OrderArticle.quantity*(100-discount)/100.0)
			FROM OrderArticle
				JOIN Article ON (OrderArticle.idArticle = Article.idArticle)
			WHERE idOrder = @orderId
			GROUP BY idShop

		DECLARE @shopId INT
		DECLARE @shopAmount DECIMAL(10,3)

		OPEN shopCursor
		FETCH NEXT FROM shopCursor INTO @shopId, @shopAmount

		WHILE @@FETCH_STATUS = 0 BEGIN
			SET @shopAmount *= @shopAmountMultiplier

			INSERT INTO [Transaction] (paymentTime, idOrder, amount) VALUES (@orderReceivedTime, @orderId, @shopAmount)
			INSERT INTO ShopTransfer (idTransaction, idShop) VALUES (@@IDENTITY, @shopId)

			FETCH NEXT FROM shopCursor INTO @shopId, @shopAmount
		END

		CLOSE shopCursor
		DEALLOCATE shopCursor

		FETCH NEXT FROM orderCursor INTO @orderId, @orderPrice, @orderReceivedTime, @systemDiscount
	END

	CLOSE orderCursor
	DEALLOCATE orderCursor
END
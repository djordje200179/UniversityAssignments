package rs.etf.sab.student;

import rs.etf.sab.operations.OrderOperations;

import java.math.BigDecimal;
import java.sql.Date;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Types;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

public class md200250_OrderOperations implements OrderOperations {
	@Override
	public int addArticle(int orderId, int articleId, int count) {
		boolean hasArticle;
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT COUNT(*) FROM OrderArticle WHERE idOrder = ? AND idArticle = ?"
		)) {
			statement.setInt(1, orderId);
			statement.setInt(2, articleId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if(resultSet.next())
					hasArticle = resultSet.getInt(1) > 0;
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		if(hasArticle) {
			try (var statement = DB.getInstance().prepareStatement(
				"UPDATE OrderArticle SET quantity = quantity + ? WHERE idArticle = ? AND idOrder = ?"
			)) {
				statement.setInt(1, count);
				statement.setInt(2, articleId);
				statement.setInt(3, orderId);

				if (statement.executeUpdate() == 0)
					return -1;

				return orderId;
			} catch (SQLException e) {
				return -1;
			}
		} else {
			try (var statement = DB.getInstance().prepareStatement(
				"INSERT INTO OrderArticle (idArticle, idOrder, quantity) VALUES (?, ?, ?)",
				Statement.RETURN_GENERATED_KEYS)
			) {
				statement.setInt(1, articleId);
				statement.setInt(2, orderId);
				statement.setInt(3, count);

				if (statement.executeUpdate() == 0)
					return -1;

				return orderId;
			} catch (SQLException e) {
				return -1;
			}
		}
	}

	@Override
	public int removeArticle(int orderId, int articleId) {
		try (var statement = DB.getInstance().prepareStatement(
			"DELETE FROM OrderArticle WHERE idOrder = ? SET idArticle = ?"
		)) {
			statement.setInt(1, orderId);
			statement.setInt(2, articleId);

			return statement.executeUpdate() == 1 ? 1 : -1;
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public List<Integer> getItems(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT idArticle FROM OrderArticle WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				var articles = new ArrayList<Integer>();
				while (resultSet.next())
					articles.add(resultSet.getInt(1));

				return articles;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public int completeOrder(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT idArticle FROM OrderArticle WHERE idOrder = ? AND (SELECT Article.quantity FROM Article WHERE Article.idArticle = OrderArticle.idArticle) < quantity"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		boolean hasDiscount;
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT COUNT(*) FROM [Order] WHERE sentTime > ? AND price > 10000 "
		)) {
			var calendar = Calendar.getInstance();
			calendar.setTime(md200250_GeneralOperations.getCalendar().getTime());
			calendar.add(Calendar.DATE, -30);
			var monthAgoDate = new Date(calendar.getTimeInMillis());

			statement.setDate(1, monthAgoDate);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					hasDiscount = resultSet.getInt(1) > 0;
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		BigDecimal totalPrice = getFinalPrice(orderId);

		try (var statement = DB.getInstance().prepareStatement(
			"SELECT balance FROM Buyer JOIN [Order] ON (Buyer.idBuyer = [Order].idBuyer) WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if(!resultSet.next() || resultSet.getBigDecimal(1).compareTo(totalPrice) < 0)
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		int transactionId;
		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO [Transaction] (idOrder, amount, paymentTime) VALUES (?, ?, ?)",
			Statement.RETURN_GENERATED_KEYS)
		) {
			statement.setInt(1, orderId);
			statement.setBigDecimal(2, totalPrice);
			statement.setDate(3, new Date(md200250_GeneralOperations.getCalendar().getTimeInMillis()));

			if (statement.executeUpdate() == 0)
				return -1;

			try (var generatedKeys = statement.getGeneratedKeys()) {
				if (generatedKeys.next())
					transactionId = generatedKeys.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		try (var statement = DB.getInstance().prepareStatement(
			"INSERT INTO OrderPayment (idTransaction) VALUES (?)"
		)) {
			statement.setInt(1, transactionId);

			statement.executeUpdate();
		} catch (SQLException e) {
			return -1;
		}

		int buyerCityId;
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT Buyer.idCity FROM [Order] JOIN Buyer ON ([Order].idBuyer = Buyer.idBuyer) WHERE [Order].idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					buyerCityId = resultSet.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		int closestCityId;
		String orderPath;
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT startCity, path, distance
			    FROM ShortestConnections
			    WHERE
			        endCity = ? AND
			        startCity IN (SELECT DISTINCT idCity FROM Shop) AND
			        distance = (
			            SELECT MIN(distance)
			            FROM ShortestConnections
			            WHERE endCity = ? AND startCity IN (SELECT DISTINCT idCity FROM Shop)
			        )
			""")) {
			statement.setInt(1, buyerCityId);
			statement.setInt(2, buyerCityId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next()) {
					closestCityId = resultSet.getInt(1);
					orderPath = resultSet.getString(2);
				} else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		try (var statement = DB.getInstance().prepareStatement("""
				UPDATE OrderArticle
			    SET
			        discount = (SELECT discount FROM Shop JOIN Article ON (Shop.idShop = Article.idShop) WHERE Article.idArticle = OrderArticle.idArticle),
			        price = (SELECT price FROM Article WHERE Article.idArticle = OrderArticle.idArticle),
			        daysLeft = (
		            SELECT distance
		            FROM ShortestConnections
		            WHERE startCity = (SELECT idCity FROM Shop JOIN Article ON (Shop.idShop = Article.idShop) WHERE Article.idArticle = OrderArticle.idArticle)
		                AND endCity = ?
		        )
			    WHERE idOrder = ?
			""")) {
			statement.setInt(1, closestCityId);
			statement.setInt(2, orderId);

			statement.executeUpdate();
		} catch (SQLException e) {
			return -1;
		}

		try (var statement = DB.getInstance().prepareStatement("""
				UPDATE Article
				SET quantity = quantity - (SELECT quantity FROM OrderArticle WHERE idOrder = ? AND OrderArticle.idArticle = Article.idArticle)
				WHERE idArticle IN (SELECT idArticle FROM OrderArticle WHERE idOrder = ?)
			""")) {
			statement.setInt(1, orderId);
			statement.setInt(2, orderId);

			statement.executeUpdate();
		} catch (SQLException e) {
			return -1;
		}

		try (var statement = DB.getInstance().prepareStatement(
			"UPDATE [Order] SET state='sent', idCity = ?, daysLeft = -1, systemDiscount = ?, path = ?, sentTime = ? WHERE idOrder = ?"
		)) {
			statement.setInt(1, closestCityId);
			statement.setInt(2, hasDiscount ? 1 : 0);
			statement.setString(3, orderPath);
			statement.setDate(4, new Date(md200250_GeneralOperations.getCalendar().getTimeInMillis()));
			statement.setInt(5, orderId);

			statement.executeUpdate();
		} catch (SQLException e) {
			return -1;
		}

		return 1;
	}

	@Override
	public BigDecimal getFinalPrice(int orderId) {
		var currentDate = new Date(md200250_GeneralOperations.getCalendar().getTimeInMillis());

		try (var statement = DB.getInstance().prepareCall(
			"{CALL dbo.SP_FINAL_PRICE(?, ?, ?)}")) {
			statement.setInt(1, orderId);
			statement.setDate(2, currentDate);
			statement.registerOutParameter(3, Types.DECIMAL);

			statement.execute();

			return statement.getBigDecimal(3).setScale(3);
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public BigDecimal getDiscountSum(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT COALESCE(SUM(price*quantity*discount/100.0), 0) FROM OrderArticle WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					return resultSet.getBigDecimal(1).setScale(3);
				else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public String getState(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT state FROM [Order] WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					return resultSet.getString(1);
				else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public Calendar getSentTime(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT sentTime FROM [Order] WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next()) {
					var date = resultSet.getDate(1);
					if (date == null)
						return null;

					var calendar = Calendar.getInstance();
					calendar.setTime(date);
					return calendar;
				} else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public Calendar getRecievedTime(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT receivedTime FROM [Order] WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next()) {
					var date = resultSet.getDate(1);
					if (date == null)
						return null;

					var calendar = Calendar.getInstance();
					calendar.setTime(date);
					return calendar;
				} else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public int getBuyer(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT idBuyer FROM [Order] WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					return resultSet.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public int getLocation(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT idCity FROM [Order] WHERE idOrder = ?"
		)) {
			statement.setInt(1, orderId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next())
					return resultSet.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}
	}
}

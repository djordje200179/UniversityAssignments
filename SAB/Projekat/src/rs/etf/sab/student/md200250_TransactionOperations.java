package rs.etf.sab.student;

import rs.etf.sab.operations.TransactionOperations;

import java.math.BigDecimal;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

public class md200250_TransactionOperations implements TransactionOperations {
	@Override
	public BigDecimal getBuyerTransactionsAmmount(int buyerId) {
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT COALESCE(SUM(amount), 0)
				FROM [Transaction] 
					JOIN OrderPayment ON ([Transaction].idTransaction = OrderPayment.idTransaction) 
					JOIN [Order] ON ([Transaction].idOrder = [Order].idOrder) 
				WHERE idBuyer = ?
			""")) {
			statement.setInt(1, buyerId);

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
	public BigDecimal getShopTransactionsAmmount(int shopId) {
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT COALESCE(SUM(amount), 0)
				FROM [Transaction]
					JOIN ShopTransfer ON ([Transaction].idTransaction = ShopTransfer.idTransaction) 
				WHERE idShop = ?
			""")) {
			statement.setInt(1, shopId);

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
	public List<Integer> getTransationsForBuyer(int buyerId) {
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT [Transaction].idTransaction
				FROM [Transaction]
					JOIN OrderPayment ON ([Transaction].idTransaction = OrderPayment.idTransaction) 
					JOIN [Order] ON ([Transaction].idOrder = [Order].idOrder) 
				WHERE idBuyer = ?
			""")) {
			statement.setInt(1, buyerId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				var orders = new ArrayList<Integer>();
				while (resultSet.next())
					orders.add(resultSet.getInt(1));

				return orders;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public int getTransactionForBuyersOrder(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT idTransaction FROM [Transaction] WHERE idOrder = ?"
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
	public int getTransactionForShopAndOrder(int orderId, int shopId) {
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT [Transaction].idTransaction
				FROM [Transaction]
					JOIN ShopTransfer ON ([Transaction].idTransaction = ShopTransfer.idTransaction)
				WHERE idOrder = ? AND idShop = ?
			""")) {
			statement.setInt(1, orderId);
			statement.setInt(2, shopId);

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
	public List<Integer> getTransationsForShop(int shopId) {
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT idTransaction
				FROM [Transaction]
					JOIN ShopTransfer ON ([Transaction].idTransaction = ShopTransfer.idTransaction) 
				WHERE idShop = ?
			""")) {
			statement.setInt(1, shopId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				var transactions = new ArrayList<Integer>();
				while (resultSet.next())
					transactions.add(resultSet.getInt(1));

				return transactions;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public Calendar getTimeOfExecution(int transactionId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT paymentTime FROM [Transaction] WHERE idTransaction = ?"
		)) {
			statement.setInt(1, transactionId);

			statement.execute();

			try (var resultSet = statement.getResultSet()) {
				if (resultSet.next()) {
					var calendar = Calendar.getInstance();
					calendar.setTime(resultSet.getDate(1));
					return calendar;
				} else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public BigDecimal getAmmountThatBuyerPayedForOrder(int orderId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT price FROM [Order] WHERE idOrder = ?"
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
	public BigDecimal getAmmountThatShopRecievedForOrder(int orderId, int shopId) {
		try (var statement = DB.getInstance().prepareStatement("""
				SELECT amount 
				FROM [Transaction]
					JOIN ShopTransfer ON ([Transaction].idTransaction = ShopTransfer.idTransaction)
					JOIN [Order] ON ([Transaction].idOrder = [Order].idOrder)
				WHERE idOrder = ? AND idShop = ?
			""")) {
			statement.setInt(1, orderId);
			statement.setInt(2, shopId);

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
	public BigDecimal getTransactionAmount(int transactionId) {
		try (var statement = DB.getInstance().prepareStatement(
			"SELECT amount FROM [Transaction] WHERE idTransaction = ?"
		)) {
			statement.setInt(1, transactionId);

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
	public BigDecimal getSystemProfit() {
		try (var statement = DB.getInstance().createStatement()) {
			try (var resultSet = statement.executeQuery("""
					SELECT COALESCE(
						SUM(IIF(
							[Order].state = 'arrived',
							price * IIF([Order].systemDiscount = 1, 0.03, 0.05),
							0
						)),
						0
					)
					FROM [Transaction]
						JOIN OrderPayment ON ([Transaction].idTransaction = OrderPayment.idTransaction)
						JOIN [Order] ON ([Order].idOrder = [Transaction].idOrder)
				""")) {
				if (resultSet.next())
					return resultSet.getBigDecimal(1).setScale(3);
				else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}
}

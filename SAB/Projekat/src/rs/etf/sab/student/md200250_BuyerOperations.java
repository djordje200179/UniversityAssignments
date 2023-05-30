package rs.etf.sab.student;

import rs.etf.sab.operations.BuyerOperations;

import java.math.BigDecimal;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class md200250_BuyerOperations implements BuyerOperations {
	@Override
	public int createBuyer(String name, int cityId) {
		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO Buyer (name, idCity) VALUES (?, ?)",
			Statement.RETURN_GENERATED_KEYS)
		) {
			statement.setString(1, name);
			statement.setInt(2, cityId);

			if (statement.executeUpdate() == 0)
				return -1;

			try (var generatedKeys = statement.getGeneratedKeys()) {
				if (generatedKeys.next())
					return generatedKeys.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public int setCity(int buyerId, int cityId) {
		try(var statement = DB.getInstance().prepareStatement(
			"UPDATE Buyer SET idCity = ? WHERE idBuyer = ?"
		)) {
			statement.setInt(1, cityId);
			statement.setInt(2, buyerId);

			return statement.executeUpdate() == 1 ? 1 : -1;
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public int getCity(int buyerId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idCity FROM Buyer WHERE idBuyer = ?"
		)) {
			statement.setInt(1, buyerId);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
				if(resultSet.next())
					return resultSet.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public BigDecimal increaseCredit(int buyerId, BigDecimal credit) {
		try(var statement = DB.getInstance().prepareStatement(
			"UPDATE Buyer SET balance = balance + ? WHERE idBuyer = ?"
		)) {
			statement.setBigDecimal(1, credit);
			statement.setInt(2, buyerId);

			if (statement.executeUpdate() == 1)
				return getCredit(buyerId).setScale(3);
			else
				return null;
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public int createOrder(int buyerId) {
		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO [Order] (idBuyer) VALUES (?)",
			Statement.RETURN_GENERATED_KEYS)
		) {
			statement.setInt(1, buyerId);

			if (statement.executeUpdate() == 0)
				return -1;

			try (var generatedKeys = statement.getGeneratedKeys()) {
				if (generatedKeys.next())
					return generatedKeys.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public List<Integer> getOrders(int buyerId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idOrder FROM [Order] WHERE idBuyer = ?"
		)) {
			statement.setInt(1, buyerId);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
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
	public BigDecimal getCredit(int buyerId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT balance FROM Buyer WHERE idBuyer = ?"
		)) {
			statement.setInt(1, buyerId);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
				if(resultSet.next())
					return resultSet.getBigDecimal(1).setScale(3);
				else
					return null;
			}
		} catch (SQLException e) {
			return null;
		}
	}
}

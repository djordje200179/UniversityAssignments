package rs.etf.sab.student;

import rs.etf.sab.operations.ShopOperations;

import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class md200250_ShopOperations implements ShopOperations {
	@Override
	public int createShop(String name, String cityName) {
		int cityId;
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idCity FROM City WHERE name = ?"
		)) {
			statement.setString(1, cityName);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
				if(resultSet.next())
					cityId = resultSet.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO Shop (name, idCity) VALUES (?, ?)",
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
	public int setCity(int shopId, String cityName) {
		int cityId;
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idCity FROM City WHERE name = ?"
		)) {
			statement.setString(1, cityName);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
				if(resultSet.next())
					cityId = resultSet.getInt(1);
				else
					return -1;
			}
		} catch (SQLException e) {
			return -1;
		}

		try(var statement = DB.getInstance().prepareStatement(
			"UPDATE Shop SET idCity = ? WHERE idShop = ?"
		)) {
			statement.setInt(1, cityId);
			statement.setInt(2, shopId);

			return statement.executeUpdate() == 1 ? 1 : -1;
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public int getCity(int shopId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idCity FROM Shop WHERE idShop = ?"
		)) {
			statement.setInt(1, shopId);

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
	public int setDiscount(int shopId, int discountPercentage) {
		try(var statement = DB.getInstance().prepareStatement(
			"UPDATE Shop SET discount = ? WHERE idShop = ?"
		)) {
			statement.setInt(1, discountPercentage);
			statement.setInt(2, shopId);

			return statement.executeUpdate() == 1 ? 1 : -1;
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public int increaseArticleCount(int articleId, int increment) {
		try(var statement = DB.getInstance().prepareStatement(
			"UPDATE Article SET quantity = quantity + ? WHERE idArticle = ?"
		)) {
			statement.setInt(1, increment);
			statement.setInt(2, articleId);

			if (statement.executeUpdate() == 1)
				return getArticleCount(articleId);
			else
				return -1;
		} catch (SQLException e) {
			return -1;
		}
	}

	@Override
	public int getArticleCount(int articleId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT quantity FROM Article WHERE idArticle = ?"
		)) {
			statement.setInt(1, articleId);

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
	public List<Integer> getArticles(int shopId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idArticle FROM Article WHERE idShop = ?"
		)) {
			statement.setInt(1, shopId);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
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
	public int getDiscount(int shopId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT discount FROM Shop WHERE idShop = ?"
		)) {
			statement.setInt(1, shopId);

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
}

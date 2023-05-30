package rs.etf.sab.student;

import rs.etf.sab.operations.ArticleOperations;

import java.sql.SQLException;
import java.sql.Statement;

public class md200250_ArticleOperations implements ArticleOperations {

	@Override
	public int createArticle(int shopId, String articleName, int articlePrice) {
		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO Article (name, idShop, price) VALUES (?, ?, ?)",
			Statement.RETURN_GENERATED_KEYS)
		) {
			statement.setString(1, articleName);
			statement.setInt(2, shopId);
			statement.setInt(3, articlePrice);

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
}

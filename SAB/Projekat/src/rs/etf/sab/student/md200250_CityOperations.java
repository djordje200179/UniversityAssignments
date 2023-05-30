package rs.etf.sab.student;

import rs.etf.sab.operations.CityOperations;

import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class md200250_CityOperations implements CityOperations {
	@Override
	public int createCity(String name) {
		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO City (name) VALUES (?)",
			Statement.RETURN_GENERATED_KEYS)
		) {
			statement.setString(1, name);

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
	public List<Integer> getCities() {
		try(var statement = DB.getInstance().createStatement()) {
			try(var resultSet = statement.executeQuery(
				"SELECT idCity FROM City"
			)) {
				var cities = new ArrayList<Integer>();
				while (resultSet.next())
					cities.add(resultSet.getInt(1));

				return cities;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public int connectCities(int cityId1, int cityId2, int distance) {
		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO Road (idCity1, idCity2, length) VALUES (?, ?, ?)")
		) {
			statement.setInt(1, cityId1);
			statement.setInt(2, cityId2);
			statement.setInt(3, distance);

			if (statement.executeUpdate() == 0)
				return -1;
		} catch (SQLException e) {
			return -1;
		}

		try(var statement = DB.getInstance().prepareStatement(
			"INSERT INTO Road (idCity1, idCity2, length) VALUES (?, ?, ?)")
		) {
			statement.setInt(1, cityId2);
			statement.setInt(2, cityId1);
			statement.setInt(3, distance);

			if (statement.executeUpdate() == 0)
				return -1;
		} catch (SQLException e) {
			return -1;
		}

		return 1;
	}

	@Override
	public List<Integer> getConnectedCities(int cityId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idCity2 FROM Road WHERE idCity1 = ?"
		)) {
			statement.setInt(1, cityId);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
				var cities = new ArrayList<Integer>();
				while (resultSet.next())
					cities.add(resultSet.getInt(1));

				return cities;
			}
		} catch (SQLException e) {
			return null;
		}
	}

	@Override
	public List<Integer> getShops(int cityId) {
		try(var statement = DB.getInstance().prepareStatement(
			"SELECT idShop FROM Shop WHERE idCity = ?"
		)) {
			statement.setInt(1, cityId);

			statement.execute();

			try(var resultSet = statement.getResultSet()) {
				var shops = new ArrayList<Integer>();
				while (resultSet.next())
					shops.add(resultSet.getInt(1));

				return shops;
			}
		} catch (SQLException e) {
			return null;
		}
	}
}

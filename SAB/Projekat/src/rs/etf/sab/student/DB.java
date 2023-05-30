package rs.etf.sab.student;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class DB {
	private static Connection connection = null;

	private static final String HOST = "localhost";
	private static final int PORT = 1433;
	private static final String DATABASE_NAME = "ShopSystem";
	private static final String USERNAME = "sa", PASSWORD = "Djole2001";

	public static Connection getInstance() {
		if (connection == null) {
			try {
				var url = String.format(
					"jdbc:sqlserver://%s:%d;databaseName=%s;encrypt=true;trustServerCertificate=true",
					HOST, PORT, DATABASE_NAME
				);

				//connection = DriverManager.getConnection(url, USERNAME, PASSWORD);
				connection = DriverManager.getConnection(url);
			} catch (SQLException e) {
				throw new RuntimeException(e);
			}
		}

		return connection;
	}
}

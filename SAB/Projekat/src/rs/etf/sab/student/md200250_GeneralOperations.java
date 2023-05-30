package rs.etf.sab.student;

import rs.etf.sab.operations.GeneralOperations;

import java.sql.Date;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Calendar;

public class md200250_GeneralOperations implements GeneralOperations {
    private static final Calendar calendar = Calendar.getInstance();

    public static Calendar getCalendar() {
        return calendar;
    }

    @Override
    public void setInitialTime(Calendar calendar) {
        md200250_GeneralOperations.calendar.setTime(calendar.getTime());
    }

    @Override
    public Calendar time(int days) {
        calendar.add(Calendar.DATE, days);

        var ordersIds = new ArrayList<Integer>();
        try (var statement = DB.getInstance().createStatement()) {
            try (var resultSet = statement.executeQuery(
                    "SELECT idOrder FROM [Order] WHERE state = 'sent'"
            )) {
                while (resultSet.next())
                    ordersIds.add(resultSet.getInt(1));
            }
        } catch (SQLException e) {
            return null;
        }

        for (var orderId : ordersIds) {
            int maxArticleDaysLeft;
            try (var statement = DB.getInstance().prepareStatement(
                    "SELECT MAX(daysLeft) FROM OrderArticle WHERE idOrder = ?"
            )) {
                statement.setInt(1, orderId);

                statement.execute();

                try (var resultSet = statement.getResultSet()) {
                    if (resultSet.next())
                        maxArticleDaysLeft = resultSet.getInt(1);
                    else
                        return null;
                }
            } catch (SQLException e) {
                return null;
            }

            if (maxArticleDaysLeft > days) {
                try (var statement = DB.getInstance().prepareStatement("""
                        UPDATE OrderArticle
                        SET daysLeft = IIF(daysLeft - ? > 0, daysLeft - ?, 0)
                        WHERE idOrder = ?
                        """)) {
                    statement.setInt(1, days);
                    statement.setInt(2, days);
                    statement.setInt(3, orderId);

                    statement.executeUpdate();
                } catch (SQLException e) {
                    return null;
                }

                return calendar;
            }

            int daysNeedToPass = days - maxArticleDaysLeft;
            if (maxArticleDaysLeft > 0) {
                try (var statement = DB.getInstance().prepareStatement("""
                        UPDATE OrderArticle
                        SET daysLeft = 0
                        WHERE idOrder = ?
                        """)) {
                    statement.setInt(1, orderId);

                    statement.executeUpdate();
                } catch (SQLException e) {
                    return null;
                }
            }

            try (var statement = DB.getInstance().prepareStatement("""
                    UPDATE [Order]
                    SET daysLeft = 0
                    WHERE idOrder = ? AND daysLeft = -1
                    """)) {
                statement.setInt(1, orderId);

                statement.executeUpdate();
            } catch (SQLException e) {
                return null;
            }

            try (var statement = DB.getInstance().prepareStatement("""
                    UPDATE [Order]
                    SET daysLeft = daysLeft - ?
                    WHERE idOrder = ?
                    """)) {
                statement.setInt(1, daysNeedToPass);
                statement.setInt(2, orderId);

                statement.executeUpdate();
            } catch (SQLException e) {
                return null;
            }

            while (true) {
                int daysLeft;
                try (var statement = DB.getInstance().prepareStatement("""
                        	SELECT daysLeft
                        	FROM [Order]
                        	WHERE idOrder = ?
                        """)) {
                    statement.setInt(1, orderId);

                    statement.execute();

                    try (var resultSet = statement.getResultSet()) {
                        if (resultSet.next())
                            daysLeft = resultSet.getInt(1);
                        else
                            return null;
                    }
                } catch (SQLException e) {
                    return null;
                }

                if (daysLeft > 0)
                    break;

                String path;
                try (var statement = DB.getInstance().prepareStatement("""
                        	SELECT path
                        	FROM [Order]
                        	WHERE idOrder = ?
                        """)) {
                    statement.setInt(1, orderId);

                    statement.execute();

                    try (var resultSet = statement.getResultSet()) {
                        if (resultSet.next())
                            path = resultSet.getString(1);
                        else
                            return null;
                    }
                } catch (SQLException e) {
                    return null;
                }

                var pathParts = path.split("\\|");
                var firstCityId = Integer.parseInt(pathParts[0]);
                if (pathParts.length == 1) {
                    var calendar = Calendar.getInstance();
                    calendar.setTime(md200250_GeneralOperations.getCalendar().getTime());
                    calendar.add(Calendar.DATE, daysLeft);

                    try (var statement = DB.getInstance().prepareStatement("""
                            	UPDATE [Order]
                            	SET daysLeft = 0, state = 'arrived', idCity = ?, receivedTime = ?
                            	WHERE idOrder = ?
                            """)) {
                        statement.setInt(1, firstCityId);
                        statement.setDate(2, new Date(calendar.getTimeInMillis()));
                        statement.setInt(3, orderId);

                        statement.executeUpdate();
                    } catch (SQLException e) {
                        return null;
                    }

                    return calendar;
                }

                var secondCityId = Integer.parseInt(pathParts[1]);
                var newPath = path.substring(path.indexOf('|') + 1);

                try (var statement = DB.getInstance().prepareStatement("""
                        	UPDATE [Order]
                        	SET daysLeft = daysLeft + (
                        		SELECT length
                        		FROM Road
                        		WHERE idCity1 = ? AND idCity2 = ?
                        	), path = ?, idCity = ?
                        	WHERE idOrder = ?
                        """)) {
                    statement.setInt(1, firstCityId);
                    statement.setInt(2, secondCityId);
                    statement.setString(3, newPath);
                    statement.setInt(4, firstCityId);
                    statement.setInt(5, orderId);

                    statement.executeUpdate();
                } catch (SQLException e) {
                    return null;
                }
            }
        }

        return calendar;
    }

    @Override
    public Calendar getCurrentTime() {
        return calendar;
    }

    @Override
    public void eraseAll() {
        try (var statement = DB.getInstance().createStatement()) {
            statement.execute("DELETE FROM ShopTransfer WHERE 1=1;");
            statement.execute("DELETE FROM OrderPayment WHERE 1=1;");
            statement.execute("DELETE FROM [Transaction] WHERE 1=1;");
            statement.execute("DELETE FROM OrderArticle WHERE 1=1;");
            statement.execute("DELETE FROM [Order] WHERE 1=1;");
            statement.execute("DELETE FROM Article WHERE 1=1;");
            statement.execute("DELETE FROM Shop WHERE 1=1;");
            statement.execute("DELETE FROM Buyer WHERE 1=1;");
            statement.execute("DELETE FROM Road WHERE 1=1;");
            statement.execute("DELETE FROM City WHERE 1=1;");
        } catch (SQLException ignored) {
        }
    }
}

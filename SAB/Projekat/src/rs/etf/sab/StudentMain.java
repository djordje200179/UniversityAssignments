package rs.etf.sab;

import rs.etf.sab.operations.*;
import rs.etf.sab.student.*;
import rs.etf.sab.tests.TestHandler;
import rs.etf.sab.tests.TestRunner;

public class StudentMain {

	public static void main(String[] args) {

		ArticleOperations articleOperations = new md200250_ArticleOperations(); // Change this for your implementation (points will be negative if interfaces are not implemented).
		BuyerOperations buyerOperations = new md200250_BuyerOperations();
		CityOperations cityOperations = new md200250_CityOperations();
		GeneralOperations generalOperations = new md200250_GeneralOperations();
		OrderOperations orderOperations = new md200250_OrderOperations();
		ShopOperations shopOperations = new md200250_ShopOperations();
		TransactionOperations transactionOperations = new md200250_TransactionOperations();

        TestHandler.createInstance(
                articleOperations,
                buyerOperations,
                cityOperations,
                generalOperations,
                orderOperations,
                shopOperations,
                transactionOperations
        );

        TestRunner.runTests();
	}
}

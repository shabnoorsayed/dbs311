/****************** DBS Assignment 2 *****************/
/****************** Shabnoor Sayed ******************/
/********************* 133649194 *********************/
/****************** November 25 2020 *****************/

#define _CRT_SECURE_NO_WARNINGS
#define _SET_SERVEROUTPUT_ON
#include <iostream>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;

using namespace oracle::occi;
using namespace std;

struct ShoppingCart {
	int product_id;
	double price;
	int quantity;
};

int mainMenu();
int getInt();
int customerLogin(Connection* conn, int customerId);
int addToCart(Connection* conn, struct ShoppingCart cart[]);
double findProduct(Connection* conn, int product_id);
int checkout(Connection* conn, struct ShoppingCart cart[], int customerId, int productCount);
void displayProducts(struct ShoppingCart cart[], int productCount);

/***** Global Variables *****/
// OCCI Variables:
Environment* env = nullptr;
Connection* conn = nullptr;
Statement* stmt = nullptr;
ResultSet* rs = nullptr;
// Credentials Used (change to your own to test):
string user = " ";
string pass = " ";
string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";


int main(void) {
	env = Environment::createEnvironment(Environment::DEFAULT);
	conn = env->createConnection(user, pass, constr);
	cout << "Connection is Successful!" << endl;
	
	try {
		// mainMenu
		int choice;
		int customerId;
		do {
			choice = mainMenu();
			if (choice == 1) {
				cout << "Enter the customer ID: ";
				customerId = getInt();
				if (customerLogin(conn, customerId) == 0) {
					cout << "The customer does not exist." << endl;
				}
				else {
					ShoppingCart cart[5];
					int productCnt = addToCart(conn, cart);
					displayProducts(cart, productCnt);
					checkout(conn, cart, customerId, productCnt);
				}
			}
		} while (choice != 0);

		cout << "Good bye!..." << endl;

		//Finally, do a select to show orders in 2020. 
		string select_order = "SELECT * FROM orders WHERE TO_CHAR(TO_DATE(order_dt,'dd-mon-yyyy'),'yyyy') = 2020;";
		stmt = conn->createStatement(select_order);
		rs = stmt->executeQuery();
		if (!rs->next())
		{
			cout << "There's no orders in 2020" << endl;
		}
		else
		{
			cout << "Order Number: " << rs->getInt(1);
			cout << "Customer ID: " << rs->getInt(3);
			cout << "Order Date: " << rs->getString(4);
			cout << "Status: " << rs->getString(5);
		}

		//Terminates and closes the connection and the environment, when the program terminates.
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

	return 0;
}

int mainMenu() {
	int choice = 0;
		cout << "******************** Main Menu ********************\n"
		<< "1)\tLogin\n"
		<< "0)\tExit\n";
		cout << "Enter an option (0-1): ";

		do {
		choice = getInt();

		if (choice != 0 && choice != 1) {
			cout << "You entered a wrong value. Enter an option (0-1): ";
		}
	} while (choice != 0 && choice != 1);

	return choice;
}

// Validates all integers (will check for empty input as well as characters and prompt again if input is not valid) 
// Added & integrated by: Larissa Plourde
int getInt() {
	int value;
	bool badEntry;
	char nextChar;
	do {
		badEntry = false;
		cin >> value;
		if (cin.fail()) {
			cout << "Entered value is not an accepted integer, try again: ";
			cin.clear();
			cin.ignore(3000, '\n');
			badEntry = true;
		}
		else {
			nextChar = cin.get();
			if (nextChar != '\n') {
				cout << "Only an integer can be accepted, try again: ";
				cin.ignore(3000, '\n');
				badEntry = true;
			}
		}
	} while (badEntry);
	return value;
}

//Before you call this function, prompt the user to enter the customer ID.
//To validate the customer ID call the find_customer() stored procedure in this function.--finished by: Larissa Plourde
int customerLogin(Connection* conn, int customerId) {
	string procedureCall = "BEGIN find_customer(:1, : 2); END;";
	stmt = conn->createStatement(procedureCall);
	stmt->setInt(1, customerId);
	int valid;
	stmt->registerOutParam(2, Type::OCCIINT, sizeof(valid));
	stmt->executeUpdate(); // call the procedure
	valid = stmt->getInt(2);

	return valid;
}

//If the customerLogin() functions return 1 (The customer ID exists), call this function.
//This function receives an OCCI pointer (a reference variable to an Oracle database) and an array of type ShoppingCart.--finished by:
int addToCart(Connection* conn, struct ShoppingCart cart[]) {
	int x = 0;
	for (int i = 0; i < 5; i++) {
		int p_id;
		int p_qty;
		ShoppingCart list;
		do {
			cout << "Enter the product ID: ";
			cin >> p_id;
			cout << isdigit(p_id);
			if (findProduct(conn, p_id) == 0) {
				cout << "The product does not exist. Try again...";
			}
		} while (findProduct(conn, p_id) == 0) {
			cout << "Product Price: " << findProduct(conn, p_id) << endl;
			cout << "Enter the product Quantity: ";
			cin >> p_qty;
			cin.clear();
			cin.ignore(200, '\n');
		}
		list.product_id = p_id;
		list.price = findProduct(conn, p_id);
		list.quantity = p_qty;
		cart[i] = list;
		while (x < 0 || x > 1) {
			cout << "Enter 1 to add more products or 0 to checkout : ";
			cin >> x;
			cin.clear();
			cin.ignore(200, '\n');
		}
		return x;
}

//This function receives an OCCI pointer (a reference variable to an Oracle database) and an integer value as the product ID.
//When the user enters the product ID in the addToCart() function, the function findProduct() is called.--finished by:
double findProduct(Connection* conn, int product_id) {
	return 0.00;
}

//This function receives an array of type ShoppingCart and the number of ordered items (products). It display the product ID, price, and quantity for products stored in the cart array.
//Call this function after the function AddToCart() to display the products added by the user to the shopping cart.--finished by:JINRUI ZHAO
void displayProducts(struct ShoppingCart cart[], int productCount) {
	if (productCount > 0) {
		double totalPrice = 0;
		cout << "------- Ordered Products ---------" << endl;
		for (int i = 0; i < productCount; ++i) {
			cout << "---Item " << i + 1 << endl;
			cout << "Product ID: " << cart[i].product_id << endl;
			cout << "Price: " << cart[i].price << endl;
			cout << "Quantity: " << cart[i].quantity << endl;
			totalPrice += cart[i].price * cart[i].quantity;
		}
		cout << "----------------------------------\nTotal: " << totalPrice << endl;
	}
}

//Call this function after the function displayProduct().
//This function receives an OCCI pointer (a reference variable to an Oracle database), an array of type ShoppingCart, an integer value as the customer ID, and an integer value as the number of ordered items (products).--finished by:
int checkout(Connection* conn, struct ShoppingCart cart[], int customerId, int productCount) {
	return 0;
}


// END OF PROGRAM

/*
******************** Stored Procedures **********************

--1.find_customer(customer_id IN NUMBER, found OUT NUMBER);
--finished by : Larissa Plourde

	CREATE OR REPLACE PROCEDURE find_customer(customer_id IN NUMBER, found OUT NUMBER) 
AS
    customer_no customers.cust_no%TYPE;
BEGIN
    
    SELECT cust_no
    INTO customer_no
    FROM customers
    WHERE cust_no = customer_id;
    
    IF customer_no = customer_id THEN
      found := 1;
    END IF;
    
EXCEPTION
    WHEN no_data_found 
    	THEN found := 0;
    WHEN TOO_MANY_ROWS
        THEN dbms_output.put_line('Too many rows have been selected');
    WHEN OTHERS
        THEN DBMS_OUTPUT.PUT_LINE ('Error!');
END find_customer;

-- Test: 
DECLARE
  found NUMBER := 0;
BEGIN
  find_customer(&employee_count, found);
  DBMS_OUTPUT.PUT_LINE('Found is: ' || found);
END;
/
--2.find_product(product_id IN NUMBER, price OUT products.prod_sell % TYPE);
--finished by :

--3.add_order(customer_id IN NUMBER, new_order_id OUT NUMBER);
--finished by : Shabnoor Sayed

	CREATE OR REPLACE PROCEDURE add_order(customer_id IN NUMBER, new_order_id OUT NUMBER)
	AS
	BEGIN
		SELECT MAX(ORDER_NO)
		INTO new_order_id
		FROM orders;

		new_order_id := new_order_id + 1;

		INSERT INTO orders
		(ORDER_NO, CUST_NO, STATUS, REP_NO, ORDER_DT)
		VALUES
		(new_order_id, customer_id, 'Shipped', 36, SYSDATE);
	END;
	/

--4.add_orderline(orderId IN orderlines.order_no % type, itemId IN orderlines.line_no % type, productId IN orderlines.prod_no % type,
	--quantity IN orderlines.qty % type, price IN orderlines.price % type);
--finished by :


*/
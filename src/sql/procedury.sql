-- name: Projekt na laboratoria z baz danych
-- date: 2015-01-03
-- author: Jakub Olczyk
-- description: 
-- Procedury składowane służące do modyfikowania bazy Northwind Traders.

/* aktualizowanie danych w bazie */
DROP PROCEDURE IF EXISTS northwind.`update_customers`;

DELIMITER $$

CREATE PROCEDURE northwind.`update_customers`(
	IN AtCustomerID VARCHAR(5),
	IN AtCompanyName VARCHAR(40),
	IN AtContactName VARCHAR(30),
	IN AtContactTitle VARCHAR(30),
	IN AtAdress VARCHAR(60),
	IN AtCity VARCHAR(15),
	IN AtRegion VARCHAR(15),
	IN AtPostalCode VARCHAR(10),
	IN AtCountry VARCHAR(15),
	IN AtPhone VARCHAR(24),
	IN AtFax VARCHAR(24)
)
BEGIN
UPDATE Customers
SET CompanyName = AtCompanyName,
	 ContactName = AtContactName,
	 ContactTitle = AtContactTitle,
	 Address = AtAdress,
	 City = AtCity,
	 Region = AtRegion,
	 PostalCode = AtPostalCode, 
	 Country = AtCountry,
	 Phone = AtPhone,
	 Fax = AtFax
WHERE CustomerID = AtCustomerID;

END$$

DELIMITER ;


/* dodawanie do bazy */
DROP PROCEDURE IF EXISTS northwind.`add_to_customers`;

DELIMITER $$

CREATE PROCEDURE northwind.`add_to_customers`(
	IN AtCustomerID VARCHAR(5),
	IN AtCompanyName VARCHAR(40),
	IN AtContactName VARCHAR(30),
	IN AtContactTitle VARCHAR(30),
	IN AtAdress VARCHAR(60),
	IN AtCity VARCHAR(15),
	IN AtRegion VARCHAR(15),
	IN AtPostalCode VARCHAR(10),
	IN AtCountry VARCHAR(15),
	IN AtPhone VARCHAR(24),
	IN AtFax VARCHAR(24)
)
BEGIN
	SET @ID = '11111';
	CALL create_custID(AtCompanyName,AtContactName,@ID);

	INSERT INTO Customers 
	VALUES (@ID,
			AtCompanyName,
			AtContactName,
			AtContactTitle,
			AtAdress,
			AtCity,
			AtRegion,
			AtPostalCode,
			AtCountry,
			AtPhone,
			AtFax);

END$$

DELIMITER ;

/* usuń z bazy */
DROP PROCEDURE IF EXISTS northwind.`delete_from_customers`;

DELIMITER $$

CREATE PROCEDURE northwind.`delete_from_customers`()
BEGIN

END$$

DELIMITER ;

/*tworzenie customerID*/
DROP PROCEDURE IF EXISTS northwind.`create_custID`;

DELIMITER $$

CREATE PROCEDURE northwind.`create_custID`(
	 IN AtCompanyName VARCHAR(40),
	 IN AtContactName VARCHAR(30),
	OUT id_ret VARCHAR(5)
)
BEGIN

	SELECT UPPER(CONCAT(SUBSTR(AtCompanyName, 1, 2),SUBSTR(AtContactName,1,3))) 
	INTO id_ret;

END$$

DELIMITER ;

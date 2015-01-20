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
	IF (AtCompanyName IS NOT NULL 
		AND AtCompanyName <> ''
		AND @ID IS NOT NULL 
		AND @ID <> '') 
	THEN
	
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
	END IF;

END$$

DELIMITER ;

/* usuń z bazy */
DROP PROCEDURE IF EXISTS northwind.`delete_from_customers`;

DELIMITER $$

CREATE PROCEDURE northwind.`delete_from_customers`(
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
	DELETE FROM Customers
	WHERE CustomerID = AtCustomerID;
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
	SET @tmp_id = '00000';
	SET @counter = 0;
	REPEAT
		IF (LENGTH(AtCompanyName) < 2 OR LENGTH(AtContactName) < 3) THEN
			SELECT UPPER(
						CONCAT(
							IF(LENGTH(SUBSTR(AtCompanyName,1,1))>0,
								SUBSTR(AtCompanyName,1,1),
								IF(
									LENGTH(
										SUBSTR(
											AtContactName,
											1+@counter,
											2+@counter)
									)>0,
								SUBSTR(AtContactName,1,2),
								SUBSTR(AtContactName,1,1))),
								SUBSTR(AtContactName,1+@counter,2+@counter)
							)
						)
			INTO @tmp_id;
		ELSE 
			SELECT UPPER(
						CONCAT(
							SUBSTR(AtCompanyName, 1+@counter, 2+@counter),
							SUBSTR(AtContactName,1+@counter,3+@counter)
						)
					) 
			INTO @tmp_id;
		END IF;
	IF (LENGTH(@tmp_id) < 5) THEN
		SELECT SUBSTR(CONCAT(@tmp_id,@tmp_id,@tmp_id),1,5) 
		INTO @tmp_id;	
	END IF;	
			SET @counter = @counter + 1;
	UNTIL @tmp_id NOT IN (SELECT CustomerID FROM Customers)
	END REPEAT;
	SET id_ret = @tmp_id;

END$$

DELIMITER ;

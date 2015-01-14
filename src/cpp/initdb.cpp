/* name : Source
 * date:  
 * author: Jakub Olczyk (jakub.olczyk@openmailbox.com)
 * license : Beerware license similar to the one below. But the beer goes to me ;) 
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include "initdb.hpp"

QSqlDatabase initdb()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");	
	db.setHostName("localhost");
	db.setDatabaseName("northwind");
	db.setUserName("jakub");
	db.setPassword("");

	return db;
}

QSqlQueryModel* get_table(QSqlDatabase* db)
{
	db->open();
	QSqlQueryModel* mod = new QSqlQueryModel();
	mod->setQuery("SELECT * FROM northwind.Customers");
	db->close();

	return mod;
}

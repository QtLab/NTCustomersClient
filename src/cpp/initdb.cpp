/* name : initdb.cpp
 * date: 2015-01-11 
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
#include <QInputDialog>
#include <QMessageBox>

QSqlDatabase initdb()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");	
	db.setHostName("localhost");
	db.setDatabaseName("northwind");
	db.setUserName(QInputDialog::getText(0,"DB User","DB username:"));
	db.setPassword(QInputDialog::getText(0,"DB User","DB password:"));

	return db;
}

QSqlQueryModel* get_table(QSqlDatabase* db)
{
	if (!db->open()){
		QMessageBox::warning(0,"DB error","connection problem!");
	}
	QSqlQueryModel* mod = new QSqlQueryModel();
	mod->setQuery("SELECT * FROM northwind.Customers");
	db->close();

	return mod;
}

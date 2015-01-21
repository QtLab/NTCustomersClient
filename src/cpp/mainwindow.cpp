/* name : mainwindow.cpp
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

#include <QTextStream>
QTextStream cout(stdout);
#include <QMessageBox>

#include "mainwindow.hpp"
#include "initdb.cpp"
#include <QDataWidgetMapper>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QItemSelectionModel>
//constructor 
MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("Northwind Customers Form");
	setFixedSize(695,500);
	northwind = initdb();
	initUI();
}

void MainWindow::initUI()
{
	// widgets that are parents to all laid out widgets
	vertiMainLayout = new QWidget(this);
	vertiMainLayout->setObjectName("vertiMainLayout");
	vertiMainLayout->setGeometry(QRect(579, 20, 101, 131));
	horizMainLayout = new QWidget(this);
	horizMainLayout->setObjectName("horizMainLayout");
	horizMainLayout->setGeometry(QRect(10, 250, 674, 72));

	//layout for the main buttons
	buttonLayout = new QVBoxLayout(vertiMainLayout);
	buttonLayout->setObjectName("buttonLayout");
	buttonLayout->setContentsMargins(0,0,0,0);

	for (int i=0; i < pushNum; ++i) {
		buttons[i] = new QPushButton(vertiMainLayout);
		buttonLayout->addWidget(buttons[i]);
	}
	buttons[upd]->setObjectName("buttons[update]");
	buttons[upd]->setText("Update");
	buttons[del]->setObjectName("buttons[delete]");
	buttons[del]->setText("Delete");
	buttons[nu]->setObjectName("buttons[new]");
	buttons[nu]->setText("New");
	buttons[clr]->setObjectName("buttons[clear]");
	buttons[clr]->setText("Clear");

	//connect signals of buttons to slots
	connect(buttons[clr],SIGNAL(clicked()),this,SLOT(clear_edits()));
	connect(buttons[upd],SIGNAL(clicked()),this,SLOT(update_db()));
	connect(buttons[nu],SIGNAL(clicked()),this,SLOT(add_db()));
	connect(buttons[del],SIGNAL(clicked()),this,SLOT(del_db()));

	// QTableView that displays the Customers Table from Northwind
	tableDisplay = new QTableView(this);
	tableDisplay->setObjectName("tableDisplay");
	tableDisplay->setGeometry(QRect(10, 10, 561, 211));
	tableDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableDisplay->setSelectionMode(QAbstractItemView::SingleSelection);
	tableDisplay->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	tableDisplay->setEditTriggers(QAbstractItemView::SelectedClicked);
	tableDisplay->horizontalHeader()->setHighlightSections(false);
	tableDisplay->setAlternatingRowColors(true);

	// connect to db to get the display
	QSqlQueryModel* customersModel = get_table(&northwind);
	tableDisplay->setModel(customersModel);
	tableDisplay->setSelectionModel(new QItemSelectionModel(customersModel));

	// handling the changes in selectionModel
    connect(tableDisplay->selectionModel(),
		   	SIGNAL(selectionChanged(const QItemSelection &, 
					const QItemSelection &)),
		   	this,
		   	SLOT(slot_SelectionChanged(const QItemSelection &,
				   	const QItemSelection &)));

	//connect showRow() with QLineEdits
	connect(tableDisplay,SIGNAL(clicked(QModelIndex)),this,SLOT(map_edits(QModelIndex)));

	//display order and label
	labels[0] = new QLabel(this);
	labels[0]->setObjectName("labels[orders]");
	labels[0]->setGeometry(QRect(10, 230, 218, 14));	
	labels[0]->setText("Order by:");

	displayOrderL = new QHBoxLayout(horizMainLayout);
	displayOrderL->setObjectName("displayOrderL");
	displayOrderL->setContentsMargins(0,0,0,0);

	customerOrder = new QVBoxLayout();
	customerOrder->setObjectName("customerOrder");
	companyOrder = new QVBoxLayout();
	companyOrder->setObjectName("companyOrder");
	contactNameOrder = new QVBoxLayout();
	contactNameOrder->setObjectName("contactNameOrder");

	displayOrderL->addLayout(customerOrder);
	displayOrderL->addLayout(companyOrder);
	displayOrderL->addLayout(contactNameOrder);
		
	for (int i=0; i < radioNum; ++i){
		orderButton[i] = new QRadioButton(horizMainLayout);
	}

	orderButton[gtcustID]->setObjectName("orderButton[gtcustID]");
	orderButton[gtcustID]->setText("> CustomerID");
	orderButton[gtcustID]->setChecked(true); //default order in tableDisplay
	orderButton[ltcustID]->setObjectName("orderButton[ltcustID]");
	orderButton[ltcustID]->setText("< CustomerID");
	orderButton[gtcompname]->setObjectName("orderButton[gtcompname]");
	orderButton[gtcompname]->setText("> Company Name");
	orderButton[ltcompname]->setObjectName("orderButton[ltcompname]");
	orderButton[ltcompname]->setText("< Company Name");
	orderButton[gtconname]->setObjectName("orderButton[gtconname]");
	orderButton[gtconname]->setText("> Contact Name");
	orderButton[ltconname]->setObjectName("orderButton[ltconname]");
	orderButton[ltconname]->setText("< Contact Name");
	
	connect(orderButton[gtcustID],SIGNAL(toggled(bool)),this,SLOT(set_sort()));
	connect(orderButton[ltcustID],SIGNAL(toggled(bool)),this,SLOT(set_sort()));
	connect(orderButton[gtcompname],SIGNAL(toggled(bool)),this,SLOT(set_sort()));
	connect(orderButton[ltcompname],SIGNAL(toggled(bool)),this,SLOT(set_sort()));
	connect(orderButton[gtconname],SIGNAL(toggled(bool)),this,SLOT(set_sort()));
	connect(orderButton[ltconname],SIGNAL(toggled(bool)),this,SLOT(set_sort()));

	customerOrder->addWidget(orderButton[gtcustID]);
	customerOrder->addWidget(orderButton[ltcustID]);

	companyOrder->addWidget(orderButton[gtcompname]);
	companyOrder->addWidget(orderButton[ltcompname]);

	contactNameOrder->addWidget(orderButton[gtconname]);
	contactNameOrder->addWidget(orderButton[ltconname]);

	inputWidget = new QWidget(this);
	inputWidget->setGeometry(QRect(10, 330, 671, 156));
	inputLayout = new QVBoxLayout(inputWidget);
	inputLayout->setObjectName("inputLayout");
	inputLayout->setContentsMargins(0,0,0,0);

	for (int i=0; i < rowNum; ++i){
		inputRow[i] = new QHBoxLayout();
		inputRow[i]->setObjectName(QString("inputRow[%1]").arg(i));
	}
	
	// inputRow[0]
	labels[1] = new QLabel(inputWidget);
	labels[1]->setObjectName("labels[custID]");
	labels[1]->setText("CustomerID");
	inputRow[0]->addWidget(labels[1]);

	custIDEdit = new QLineEdit(inputWidget);
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(
			custIDEdit->sizePolicy().hasHeightForWidth());
    custIDEdit->setSizePolicy(sizePolicy1);
	custIDEdit->setReadOnly(true);
	inputRow[0]->addWidget(custIDEdit);

	labels[2] = new QLabel(inputWidget);
	labels[2]->setObjectName("labels[compName]");
	labels[2]->setText("Company Name");
	inputRow[0]->addWidget(labels[2]);

	compNameEdit = new QLineEdit(inputWidget);
	inputRow[0]->addWidget(compNameEdit);
	
	inputLayout->addLayout(inputRow[0]);
	
	// inputRow[1]
	labels[3] = new QLabel(inputWidget);
	labels[3]->setObjectName("labels[conTitle]");
	labels[3]->setText("Contact Title");
	inputRow[1]->addWidget(labels[3]);

	conTitleEdit = new QLineEdit(inputWidget);
	inputRow[1]->addWidget(conTitleEdit);

	labels[4] = new QLabel(inputWidget);
	labels[4]->setObjectName("labels[conName]");
	labels[4]->setText("Contact Name");
	inputRow[1]->addWidget(labels[4]);

	conNameEdit = new QLineEdit(inputWidget);
	inputRow[1]->addWidget(conNameEdit);
	
	inputLayout->addLayout(inputRow[1]);
	
	// inputRow[2]
	labels[5] = new QLabel(inputWidget);
	labels[5]->setObjectName("labels[address]");
	labels[5]->setText("Address");
	inputRow[2]->addWidget(labels[5]);

	addressEdit = new QLineEdit(inputWidget);
	inputRow[2]->addWidget(addressEdit);

	labels[6] = new QLabel(inputWidget);
	labels[6]->setObjectName("labels[country]");
	labels[6]->setText("Country");
	inputRow[2]->addWidget(labels[6]);

	countryEdit = new QLineEdit(inputWidget);
	sizePolicy1.setHeightForWidth(countryEdit->sizePolicy().hasHeightForWidth());
	countryEdit->setSizePolicy(sizePolicy1);
	inputRow[2]->addWidget(countryEdit);

	inputLayout->addLayout(inputRow[2]);
	
	//inputRow[3]
	labels[7] = new QLabel(inputWidget);
	labels[7]->setObjectName("labels[city]");
	labels[7]->setText("City");
	inputRow[3]->addWidget(labels[7]);

	cityEdit = new QLineEdit(inputWidget);
	inputRow[3]->addWidget(cityEdit);

	labels[8] = new QLabel(inputWidget);
	labels[8]->setObjectName("labels[region]");
	labels[8]->setText("Region");
	inputRow[3]->addWidget(labels[8]);

	regionEdit = new QLineEdit(inputWidget);
	inputRow[3]->addWidget(regionEdit);


	labels[9] = new QLabel(inputWidget);
	labels[9]->setObjectName("labels[postCode]");
	labels[9]->setText("Postal Code");
	inputRow[3]->addWidget(labels[9]);

	postCodeEdit = new QLineEdit(inputWidget);
	inputRow[3]->addWidget(postCodeEdit);

	inputLayout->addLayout(inputRow[3]);
	//inputRow[4]
	labels[10] = new QLabel(inputWidget);
	labels[10]->setObjectName("labels[phone]");
	labels[10]->setText("Phone");
	inputRow[4]->addWidget(labels[10]);

	phoneEdit = new QLineEdit(inputWidget);
	inputRow[4]->addWidget(phoneEdit);

	labels[11] = new QLabel(inputWidget);
	labels[11]->setObjectName("labels[fax]");
	labels[11]->setText("Fax");
	inputRow[4]->addWidget(labels[11]);

	faxEdit = new QLineEdit(inputWidget);
	inputRow[4]->addWidget(faxEdit);
	
	inputLayout->addLayout(inputRow[4]);
}

void MainWindow::clear_edits()
{
	cout << "\t>>SLOT: clear_edits"<<endl;
	foreach(QLineEdit* le, findChildren<QLineEdit*>()) {
		le->clear();
	}	
}

void MainWindow::map_edits(const QModelIndex& index)
{
	cout << "\t>>SLOT: map_edits"<<endl;
	QDataWidgetMapper* mapper = new QDataWidgetMapper(this);	
	mapper->setModel(this->tableDisplay->model());

	mapper->addMapping(custIDEdit,0);
	mapper->addMapping(compNameEdit,1);
	mapper->addMapping(conNameEdit,2);
	mapper->addMapping(conTitleEdit,3);
	mapper->addMapping(addressEdit,4);
	mapper->addMapping(cityEdit,5);
	mapper->addMapping(regionEdit,6);
	mapper->addMapping(postCodeEdit,7);
	mapper->addMapping(countryEdit,8);
	mapper->addMapping(phoneEdit,9);
	mapper->addMapping(faxEdit,10);
	mapper->setCurrentModelIndex(index);
}

// update database
void MainWindow::update_db()
{
	cout << "\t>>SLOT: update_db"<<endl;
	cout  << ( northwind.open() ? "connected...": "not connected") << endl;
	cout << custIDEdit->text() <<endl;
	QSqlQuery query;
	query.exec(
			QString("CALL update_customers(\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\',\'%10\',\'%11\')")
			.arg(custIDEdit->text())
			.arg(compNameEdit->text())
			.arg(conNameEdit->text())
			.arg(conTitleEdit->text())
			.arg(addressEdit->text())
			.arg(cityEdit->text())
			.arg(regionEdit->text())
			.arg(postCodeEdit->text())
			.arg(countryEdit->text())
			.arg(phoneEdit->text())
			.arg(faxEdit->text())
			);

	cout << query.lastQuery()<<endl;
	query.next();

	northwind.close();
	cout << "\tconnection closed..." << endl;
	
	cout << "Update tableDisplay" <<endl;
	cout << "\t>>CALLING SLOT:set_sort from update_db" << endl;
	set_sort();	
}

// add to database
void MainWindow::add_db()
{
	cout << "\t>>SLOT:add_db" << endl;
	cout  << ( northwind.open() ? "\tconnected...": "\tnot connected") << endl;
	QSqlQuery query;
	query.exec(
		QString("CALL add_to_customers(\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\',\'%10\',\'%11\')")	
			.arg(custIDEdit->text())
			.arg(compNameEdit->text())
			.arg(conNameEdit->text())
			.arg(conTitleEdit->text())
			.arg(addressEdit->text())
			.arg(cityEdit->text())
			.arg(regionEdit->text())
			.arg(postCodeEdit->text())
			.arg(countryEdit->text())
			.arg(phoneEdit->text())
			.arg(faxEdit->text())
			);

		cout << query.lastQuery() <<endl;
	query.next();
	northwind.close();
	cout << "\tconnection closed..." << endl;

	cout << "Update tableDisplay" <<endl;
	cout << "\t>>CALLING SLOT:set_sort from add_db" << endl;
	set_sort();
}

void MainWindow::del_db()
{
	cout << "\t>>SLOT:del_db" <<endl;
	cout  << ( northwind.open() ? "connected...": "not connected") << endl;
	QSqlQuery query;
	query.exec(
		QString("CALL delete_from_customers(\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\',\'%10\',\'%11\')")	
			.arg(custIDEdit->text())
			.arg(compNameEdit->text())
			.arg(conNameEdit->text())
			.arg(conTitleEdit->text())
			.arg(addressEdit->text())
			.arg(cityEdit->text())
			.arg(regionEdit->text())
			.arg(postCodeEdit->text())
			.arg(countryEdit->text())
			.arg(phoneEdit->text())
			.arg(faxEdit->text())
			);

	cout << query.lastQuery() <<endl;
	query.next();
	northwind.close();
	cout << "\tconnection closed..." << endl;
	cout << "Update tableDisplay" <<endl;
	cout << "\t>>CALLING SLOT:set_sort from del_db" << endl;
	set_sort(); // updating the tableDisplay with new data from db
}

void MainWindow::set_sort()
{
	if (orderButton[gtcustID]->isChecked()){
		get_sorted(0,Qt::AscendingOrder);
	}else if (orderButton[ltcustID]->isChecked()){
		get_sorted(0, Qt::DescendingOrder);
	}else if (orderButton[gtcompname]->isChecked()){
		get_sorted(1,Qt::AscendingOrder);
	}else if (orderButton[ltcompname]->isChecked()){
		get_sorted(1, Qt::DescendingOrder);
	}else if (orderButton[gtconname]->isChecked()){
		get_sorted(2,Qt::AscendingOrder);
	}else if (orderButton[ltconname]->isChecked()){
		get_sorted(2,Qt::DescendingOrder);
	}
}

void MainWindow::get_sorted(int column, Qt::SortOrder order)
{
	cout << "--------------------------------------------------------" << endl;
	cout << "\t>>FUNC: get_sorted" << endl;
	if (!this->northwind.open()){
		QMessageBox::warning(0,"DB error","connection problem!");
	}
	cout <<"\t:connected..." << endl;
	QSqlQueryModel* mod = (QSqlQueryModel*)tableDisplay->model();
	mod->setQuery(QString("SELECT * FROM northwind.Customers ORDER BY %1 %2")
					.arg((column == 0)?
							"CustomerID":
							(column == 1)?
								"CompanyName":"ContactName")
					.arg((order == Qt::AscendingOrder)? "ASC":"DESC")
			);
	cout << "\t:data from db collected" << endl;
	this->northwind.close();
	cout << "\t:connection closed..." << endl;
	this->tableDisplay->setModel(mod);
	cout << "SELECTIONMODEL changes"<<endl;
	this->tableDisplay->keyboardSearch(this->custIDEdit->text());
	cout <<  "\t\t>>DEBUG:keyboardSearch " << this->custIDEdit->text()<< endl;
	this->tableDisplay->keyboardSearch(this->compNameEdit->text());
	cout << "\t\t>>DEBUG:keyboardSearch " << this->compNameEdit->text() << endl;
	cout << "tableDisplay updated"<<endl;
	//tableDisplay->activated(this->tableDisplay->currentIndex());
	// after keyboardSearch the currentIndex index should be set
	// scrollTo will provide the control over the posision of index
	//this->tableDisplay->scrollTo(
			//this->tableDisplay->currentIndex(),
			//QTableView::PositionAtCenter);
	//this->tableDisplay->selectRow(this->tableDisplay->selectionModel()->currentIndex().row());
}

void MainWindow::slot_SelectionChanged(const QItemSelection &, const QItemSelection &)
{
	  QModelIndexList indexes = tableDisplay->selectionModel()->selection().indexes();
	  for (int i = 0; i < indexes.count(); ++i)
	  {
		// Output the rows of all of the selected indexes
		cout << "selectedIndexes: " << indexes.at(i).row() << endl;
	  }
	cout << "\t>>SLOT:slot_SelectionChanged"<<endl;	
	QModelIndex indx = tableDisplay->currentIndex();
	QItemSelection slctn;
	slctn.select(indx,indx);

	tableDisplay->selectionModel()->select(slctn,
			QItemSelectionModel::SelectCurrent |QItemSelectionModel::Rows);
	tableDisplay->selectionModel()->setCurrentIndex(indx,
			QItemSelectionModel::SelectCurrent |QItemSelectionModel::Rows);
	//tableDisplay->selectRow(tableDisplay->selectionModel()->selection().indexes().at(0).row());
	//tableDisplay->selectionModel()->select(
					//tableDisplay->selectionModel()->selection(), 
					//QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

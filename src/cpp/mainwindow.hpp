/* name: MainWindow.hpp
 * date: 2015-01-11
 * author: Jakub Olczyk (jakub.olczyk@openmailbox.org)
 * license: Beerware (rev 42)
 */
#ifndef JO_MAINWINDOW_HPP
#define JO_MAINWINDOW_HPP

#include "initdb.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QSqlDatabase>

class MainWindow : public QWidget {

	Q_OBJECT

	public:
		
		MainWindow(QWidget* parent = 0);
		
	public slots:
		void clear_edits();
		void map_edits(const QModelIndex&);
		void update_db();
		void add_db();
		void del_db();
		void set_sort();

	private:
		void initUI();

		enum { pushNum = 4,rowNum = 5, radioNum = 6, labelNum = 12};
		enum button{upd=0, del=1, nu=2, clr=3};
		enum radiobutton{
						gtcustID   = 0,
						ltcustID   = 1,
						gtcompname = 2,
						ltcompname = 3,
						gtconname  = 4,
						ltconname  = 5};

		QWidget* vertiMainLayout;
		QWidget* horizMainLayout;
		QWidget* inputWidget;
		
		QTableView* tableDisplay;	

		QVBoxLayout* buttonLayout;
		QPushButton* buttons[pushNum];
	

		QHBoxLayout* displayOrderL;
		QVBoxLayout* customerOrder;
		QVBoxLayout* companyOrder;
		QVBoxLayout* contactNameOrder;
		QRadioButton* orderButton[radioNum];

		QVBoxLayout* inputLayout;
		QHBoxLayout* inputRow[rowNum];
		QLineEdit* custIDEdit;
		QLineEdit* compNameEdit;
		QLineEdit* conTitleEdit;
		QLineEdit* conNameEdit;
		QLineEdit* addressEdit;
		QLineEdit* countryEdit;
		QLineEdit* cityEdit;
		QLineEdit* regionEdit;
		QLineEdit* postCodeEdit;
		QLineEdit* phoneEdit;
		QLineEdit* faxEdit;
		QLabel* labels[labelNum];

		QSqlDatabase northwind;
};
#endif //JO_MAINWINDOW_HPP

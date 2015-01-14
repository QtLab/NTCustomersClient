/* name : main.cpp
 * date:  2015-01-11
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

#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app (argc,argv);
	MainWindow w;
	w.show();
	return app.exec();
}

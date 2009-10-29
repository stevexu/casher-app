#include "financemgr.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FinanceMgr w;
	w.show();
	return a.exec();
}

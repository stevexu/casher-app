#include "qmysql.h"
#include "tableview.h"
#include "mainbuttonlist.h"
#include "mainview.h"
#include "ViewIdentify.h"
#include "databasehandler.h"
#include <QtGui>
#include <QCompleter>

#include "financemgr.h"

FinanceMgr::FinanceMgr(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	MainButtonList* btnList = new MainButtonList(this);
	
	// set splitter window as the central widget of main window
	mainView = new MainView(this);
	mainView->openWindow(DepartmentManagerView);
	QSplitter* splitter = new QSplitter(this);
	setCentralWidget(splitter);

	splitter->addWidget(btnList);
	splitter->addWidget(mainView);

	// connect the manage menu
	connect(ui.actionDepartment, SIGNAL(triggered()), this, SLOT(handleActionDepartment()));
	connect(ui.actionBankAccount, SIGNAL(triggered()), this, SLOT(handleActionBank()));
	connect(ui.actionBudget, SIGNAL(triggered()), this, SLOT(handleBudget()));
	connect(ui.actionTitle, SIGNAL(triggered()), this, SLOT(handleTitle()));
	connect(ui.actionDetailTitle, SIGNAL(triggered()), this, SLOT(handleDetailTitle()));
	connect(ui.actionIncome, SIGNAL(triggered()), this, SLOT(handleIncome()));
	connect(ui.actionCheck, SIGNAL(triggered()), this, SLOT(handleCRCheck()));
	connect(ui.actionCash, SIGNAL(triggered()), this, SLOT(hanldeCashShow()));

	// here connect the operation of operation menu
	connect(ui.actionInsert, SIGNAL(triggered()), this, SLOT(addItem()));
	connect(ui.actionEdit, SIGNAL(triggered()), this, SLOT(editItem()));
	connect(ui.actionDel, SIGNAL(triggered()), this, SLOT(delItem()));

	// create the toolbar
	createToolbar();

	QString strCurrentYear = QString("%1").arg(QDate::currentDate().year());
	QString strCurrentMonth = QString("%1").arg(QDate::currentDate().month());
	QString strTitle = windowTitle();
	strTitle += QString("-%1%2").arg(strCurrentYear, strCurrentMonth);
	setWindowTitle(strTitle);
}

FinanceMgr::~FinanceMgr()
{

}

void FinanceMgr::handleMonthChanged(const QString& text)
{
	// here to reload the monthly date
	// TODO: finish this methodd
}

void FinanceMgr::createToolbar()
{
	QToolBar* toolBar = addToolBar(QString::fromLocal8Bit("系统工具"));
	QAction* calcAction = new QAction(QIcon(tr(":/Icon/Resources/calculator.png")), QString::fromLocal8Bit("计算器"), this);
	toolBar->addAction(calcAction);
	connect(calcAction, SIGNAL(triggered()), this, SLOT(openCalc()));

	ui.toolBar->addWidget(new QLabel(QString::fromLocal8Bit("年月: "), this));
	QComboBox* yearMonth = new QComboBox(this);
	QStringList dateList;
	// find the most faraway month from the budget table
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(BudgetTableName)->limit("1")->orderBy("budget_month ASC")->get();
	int currentYear = QDate::currentDate().year();
	QString strCurrentYear = QString("%1").arg(currentYear);
	int currentMonth = QDate::currentDate().month();
	QString strCurrentMonth = QString("%1").arg(currentMonth);
	QString strYM = QString("%1%2").arg(strCurrentYear, strCurrentMonth);
	
	QDate databaseDate = QDate::currentDate();
	if (dr.count() > 0)
	{
		Array record = *dr.resultArray().begin();
		databaseDate = QDate::fromString(record[budgetMonthColumnName].c_str(), Qt::ISODate);
	}
	// create the year month list from the database time to current time
	bool loop = true;
	int year, month;
	year = databaseDate.year();
	month = databaseDate.month();
	do 
	{
		QString strYear = QString("%1").arg(year);
		QString strMonth = QString("%1").arg(month);
		QString strYear2Month  = QString("%1%2").arg(strYear, strMonth);
		dateList << strYear2Month;

		month++;
		if (month > 12)
		{
			month = 1;
			year++;
		}

		// add more than 2 month to the list
		int monthCount1 = year * 12 + month;
		int monthCount2 = QDate::currentDate().year() * 12 + QDate::currentDate().month();

		if ((monthCount2 + 2) < monthCount1)
		{
			loop = false;
		}
	} while (loop);

	yearMonth->addItems(dateList);
	yearMonth->setEditable(true);
	QCompleter* ymCompleter = new QCompleter(dateList, this);
	yearMonth->setCompleter(ymCompleter);
	yearMonth->setEditText(strYM);

	ui.toolBar->addWidget(yearMonth);
	connect(yearMonth, SIGNAL(editTextChanged(const QString&)), this, SLOT(handleMonthChanged(const QString&)));
}

void FinanceMgr::openCalc()
{
	// open the system calculator
	QProcess::startDetached(tr("calc.exe"));
}

void FinanceMgr::hanldeCashShow()
{
	mainView->openWindow(CashView);
}

void FinanceMgr::handleCRCheck()
{
	mainView->openWindow(CRCheckView);
}

void FinanceMgr::handleIncome()
{
	mainView->openWindow(IncomeView);
}

void FinanceMgr::handleDetailTitle()
{
	mainView->openWindow(DetailTitleView);
}

void FinanceMgr::handleTitle()
{
	mainView->openWindow(TitleView);
}

void FinanceMgr::handleBudget()
{
	mainView->openWindow(BudgetView);
}

void FinanceMgr::handleActionDepartment()
{
	mainView->openWindow(DepartmentManagerView);
}

void FinanceMgr::handleActionBank()
{
	mainView->openWindow(BankAccountView);
}

void FinanceMgr::addItem()
{
	mainView->addItem();
}

void FinanceMgr::editItem()
{
	mainView->editItem();
}

void FinanceMgr::delItem()
{
	mainView->delItem();
}

std::string arrayToString(const Array& ary)
{
	std::string str;

	for (Array::const_iterator itor = ary.begin(); itor != ary.end(); ++itor)
	{
		str += itor->first + ":";
		str += itor->second + "---";
	}

	return str;
}

void FinanceMgr::testClick(){
#if 0
	DepMgrUi ui(this);
	ui.show();


	Database* mysql = new QMySql;
	bool ok = mysql->connect("127.0.0.1", 3306, "gdf", "root", "");
	if (ok)
	{
		QMessageBox::about(this, tr("FinaceMgr"), tr("connect ok"));
	}

	Array x;
	x["money_direction"] = "D";
	x["title_code"] = "fadf3344";
	x["title_description"] = "中文测试";
	//mysql->insert("subtitle", x);
	//return;

	Database_Result& dr = mysql->query("SELECT * FROM subtitle WHERE id > 3");
	//Database_Result& dr = mysql->from("subtitle")->where("id > 3")->limit("1")->get();

	Array values;
	values["money_direction"] = "F";
	values["title_code"] = "vu";

	Array wheres;
	wheres["id"] = "3";
	bool bok = mysql->update("subtitle", values, wheres);
	mysql->del("subtitle", wheres);


	if ( (!dr.isValid()) || !dr.hasRecord())
	{
		QMessageBox::about(this, tr("FinaceMgr"), tr("query error:%1").arg(dr.errorMsg().c_str()));
		return;
	}

	ResultArray& rAry = dr.resultArray();
	foreach(Array ary, rAry)
	{
		QMessageBox::about(NULL, "test click", arrayToString(ary).c_str());
	}

#endif
}


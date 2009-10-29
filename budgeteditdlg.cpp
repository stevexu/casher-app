#include "databasehandler.h"
#include "budgeteditdlg.h"

#include <QCompleter>

BudgetEditDlg::BudgetEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	// load the title and sub title table
	QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	QRecords subTitleRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
	QStringList titleWorldList, detailTitleWorldList;

	for (int i = 0; i < titleRecords.count(); i++)
	{
		titleWorldList << (titleRecords.at(i)[titleCodeColumnName]);
	}

	for (int i = 0; i < subTitleRecords.count(); i++)
	{
		detailTitleWorldList << (subTitleRecords.at(i)[detailTitleCodeColumnName]);
	}

	ui.budgetTitle->addItems(titleWorldList);
	ui.budgetSubtitle->addItems(detailTitleWorldList);

	QCompleter* titleCompleter = new QCompleter(titleWorldList, this);
	QCompleter* detailTitleCompleter = new QCompleter(detailTitleWorldList, this);

	ui.budgetTitle->setCompleter(titleCompleter);
	ui.budgetTitle->lineEdit()->setCompleter(titleCompleter);
	ui.budgetSubtitle->setCompleter(detailTitleCompleter);
	ui.budgetSubtitle->lineEdit()->setCompleter(detailTitleCompleter);

	// set current date to budget date 
	QDateTime currentTime = QDateTime::currentDateTime();
	ui.budgetDate->setDate(currentTime.date());

	QIntValidator* intValidator = new QIntValidator(0, 2147483642, this); 
	ui.budgetId->setValidator(intValidator);
}

BudgetEditDlg::~BudgetEditDlg()
{

}

void BudgetEditDlg::setEditMode()
{
	ui.budgetId->setEnabled(false);
	ui.budgetDate->setEnabled(false);
}

void BudgetEditDlg::accept()
{
	QDialog::accept();
}

void BudgetEditDlg::setBanlanceDownDate(const QDate& date)
{
	//ui.banlanceDownDate->setDate(date);
	Q_UNUSED(date)
}

QDate BudgetEditDlg::balanceDownDate() const
{
	//return ui.banlanceDownDate->date();
	return QDate();
}

void BudgetEditDlg::setBudgetDate(const QDate& date)
{
	ui.budgetDate->setDate(date);
}

QDate BudgetEditDlg::budgetDate() const
{
	return ui.budgetDate->date();
}

void BudgetEditDlg::setBudgetId(const QString &id)
{
	ui.budgetId->setText(id);
}

QString BudgetEditDlg::budgetId() const
{
	return ui.budgetId->text();
}

void BudgetEditDlg::setBudgetMemo(const QString& memo)
{
	ui.budgetMemo->setText(memo);
}

QString BudgetEditDlg::budgetMemo() const
{
	return ui.budgetMemo->text();
}

void BudgetEditDlg::setBudgetTitle(const QString &title)
{
	ui.budgetTitle->setEditText(title);
}

QString BudgetEditDlg::budgetTitle() const
{
	return ui.budgetTitle->currentText();
}

void BudgetEditDlg::setBudgetSubTitle(const QString &subTitle)
{
	ui.budgetSubtitle->setEditText(subTitle);
}

QString BudgetEditDlg::budgetSubTitle() const
{
	return ui.budgetSubtitle->currentText();
}

void BudgetEditDlg::setBudget(const QString& budget)
{
	ui.budgetAmount->setText(budget);
}

QString BudgetEditDlg::budget() const
{
	return ui.budgetAmount->text();
}

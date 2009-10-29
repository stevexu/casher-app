#include <QCompleter>
#include "databasehandler.h"
#include "incomeeditdlg.h"

IncomeEditDlg::IncomeEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QString strYear = QString("%1").arg(QDate::currentDate().year());
	strYear.remove(0, 2);
	QString strMonth = QString("%1").arg(QDate::currentDate().month());
	QString strDatePart = QString("S%1%2").arg(strYear, strMonth);
	// get the last 1 record from the income table
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(IncomeTableName)->limit("1")->orderBy("`id` desc")->get();
	if (dr.count() > 0)
	{
		// has record in the database
		ResultArray result_array = dr.resultArray();
		Array topRecord = *result_array.begin();
		// calculate the income code
		QString dateOfLastRecord = QString::fromUtf8(topRecord[incomeCodeColumnName].substr(0, 5).c_str());	// 5 is the length of the SyyMM(S0910)

		if (dateOfLastRecord == strDatePart)
		{
			// this month has record yet
			QString seqNumberOfLastRecord = QString::fromUtf8(topRecord[incomeCodeColumnName].substr(6, 4).c_str()); // 6 is the position of the number (S0910-0001)
			int number = seqNumberOfLastRecord.toInt();
			QString strNewSeq = QString("%1").arg(number + 1, 4, 10, QChar('0'));

			QString strNewSeqCode = strDatePart + "-";
			strNewSeqCode += strNewSeq;
			ui.code->setText(strNewSeqCode);
		}
		else
		{
			QString strNewSeqCode = QString("%1-0001").arg(strDatePart);
			ui.code->setText(strNewSeqCode);
		}
	}
	else
	{
		// no record in the database
		QString strNewSeqCode = QString("%1-0001").arg(strDatePart);
		ui.code->setText(strNewSeqCode);
	}
	
	// load the title and subtitle table
	m_titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	m_accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);

	QStringList titleWorldList, accountList, titleMemoList, accountMemoList;

	for (int i = 0; i < m_titleRecords.count(); i++)
	{
		titleWorldList << (m_titleRecords.at(i)[titleCodeColumnName]);
		titleMemoList << (m_titleRecords.at(i)[titleMemoColumnName]);
	}

	for (int i = 0; i < m_accountRecords.count(); i++)
	{
		accountList << m_accountRecords.at(i)[accountCodeColumnName];
		accountMemoList << m_accountRecords.at(i)[bankNameColumnName];
	}

	ui.account->addItems(accountMemoList);
	ui.title->addItems(titleMemoList);

	QCompleter* titleCompleter = new QCompleter(titleMemoList, this);
	QCompleter* accountCompleter = new QCompleter(accountMemoList, this);

	ui.title->setCompleter(titleCompleter);
	ui.account->setCompleter(accountCompleter);

	ui.account->setCurrentIndex(-1);
	ui.title->setCurrentIndex(-1);
	
	// default is current date
	ui.date->setDate(QDate::currentDate());

	QIntValidator* intValidator = new QIntValidator(0, 200000000, this);
	ui.money->setValidator(intValidator);
}

IncomeEditDlg::~IncomeEditDlg()
{

}

void IncomeEditDlg::setEditMode()
{
	ui.code->setEnabled(false);
	ui.date->setEnabled(false);
}

void IncomeEditDlg::setCode(const QString& incomeCode)
{
	ui.code->setText(incomeCode);
}

QString IncomeEditDlg::code() const
{
	return ui.code->text();
}

void IncomeEditDlg::setDate(const QDate &incomeDate)
{
	ui.date->setDate(incomeDate);
}

QDate IncomeEditDlg::date() const
{
	return ui.date->date();
}

// here set the title's memo
void IncomeEditDlg::setTitle(const QString &incomeTitle)
{
	ui.title->setEditText(incomeTitle);
}

// return the title's code
QString IncomeEditDlg::title() const
{
	for (int i = 0; i < m_titleRecords.count(); i++)
	{
		if (m_titleRecords.at(i)[titleMemoColumnName] == ui.title->currentText())
		{
			return m_titleRecords.at(i)[titleCodeColumnName];
		}
	}
	return QString();
}

void IncomeEditDlg::setMemo(const QString &incomeMemo)
{
	ui.memo->setText(incomeMemo);
}

QString IncomeEditDlg::memo() const
{
	return ui.memo->text();
}

void IncomeEditDlg::setMoney(const QString &incomeMoney)
{
	ui.money->setText(incomeMoney);
}

QString IncomeEditDlg::money() const
{
	return ui.money->text();
}

// here set the account's bank name
void IncomeEditDlg::setAccount(const QString& incomeAccount)
{
	ui.account->setEditText(incomeAccount);
}

// return account's code
QString IncomeEditDlg::account() const{
	for (int i = 0; i < m_accountRecords.count(); i++)
	{
		if (m_accountRecords.at(i)[bankNameColumnName] == ui.account->currentText())
		{
			return m_accountRecords.at(i)[accountCodeColumnName];
		}
	}
	return QString();
}

void IncomeEditDlg::accept()
{
	QDialog::accept();
}
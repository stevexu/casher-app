#include <QCompleter>
#include "databasehandler.h"
#include "crcheckeditdlg.h"

CRCheckEditDlg::CRCheckEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.needCheck, SIGNAL(stateChanged()), this, SLOT(needCheckStateChanged()));

	QString strYear = QString("%1").arg(QDate::currentDate().year());
	strYear.remove(0, 2);
	QString strMonth = QString("%1").arg(QDate::currentDate().month());
	QString strDatePart = QString("C%1%2").arg(strYear, strMonth);
	// get the last 1 record from the income table
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(CRCheckTableName)->limit("1")->orderBy("`id` desc")->get();
	if (dr.count() > 0)
	{
		// has record in the database
		ResultArray result_array = dr.resultArray();
		Array topRecord = *result_array.begin();
		// calculate the income code
		QString dateOfLastRecord = QString::fromUtf8(topRecord[checkCodeColumnName].substr(0, 5).c_str());	// 5 is the length of the CyyMM(S0910)

		if (dateOfLastRecord == strDatePart)
		{
			// this month has record yet
			QString seqNumberOfLastRecord = QString::fromUtf8(topRecord[checkCodeColumnName].substr(6, 4).c_str()); // 6 is the position of the number (C0910-0001)
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

	// set the completer for the account, title and subtitle combo box
	m_accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);
	m_titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	m_subTitleRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
	m_depRecords = DatabaseHandler::instance()->loadTable(DepartmentTableName);

	QStringList accountList, titleList, subTitleList, depList;

	for (int i = 0; i < m_accountRecords.count(); i++)
	{
		accountList << m_accountRecords.at(i)[bankNameColumnName];
	}

	for (int i = 0; i < m_titleRecords.count(); i++)
	{
		titleList << m_titleRecords.at(i)[titleMemoColumnName];
	}

	for (int i = 0; i < m_subTitleRecords.count(); i++)
	{
		subTitleList << m_subTitleRecords.at(i)[detailTitleMemoColumnName];
	}

	for (int i = 0; i < m_depRecords.count(); i++)
	{
		depList << m_depRecords.at(i)[depMemoColumnName];
	}

	ui.CRCheckTitle->addItems(titleList);
	ui.CRCheckSubTitle->addItems(subTitleList);
	ui.department->addItems(depList);
	ui.account->addItems(accountList);

	QCompleter* titleCompleter = new QCompleter(titleList, this);
	QCompleter* subTitleCompleter = new QCompleter(subTitleList, this);
	QCompleter* depCompleter = new QCompleter(depList, this);
	QCompleter* accountCompleter = new QCompleter(accountList, this);

	ui.CRCheckSubTitle->setCompleter(subTitleCompleter);
	ui.CRCheckSubTitle->setCurrentIndex(-1);

	ui.CRCheckTitle->setCompleter(titleCompleter);
	ui.CRCheckTitle->setCurrentIndex(-1);

	ui.account->setCompleter(accountCompleter);
	ui.account->setCurrentIndex(-1);

	ui.department->setCompleter(depCompleter);
	ui.department->setCurrentIndex(-1);

	// set the current date
	ui.dateEdit->setDate(QDate::currentDate());

	QIntValidator* intValidator = new QIntValidator(0, 200000000, this);
	ui.money->setValidator(intValidator);
}

CRCheckEditDlg::~CRCheckEditDlg()
{

}

void CRCheckEditDlg::needCheckStateChanged()
{
	if (ui.needCheck->checkState() == Qt::Checked)
	{
		// show the check edit option

	}
	else
	{
		// hide the check edit option

	}
}


void CRCheckEditDlg::setAccount(const QString &checkAccount)
{
	ui.account->setEditText(checkAccount);
}

// return the account's code
QString CRCheckEditDlg::account() const
{
	for (int i = 0; i < m_accountRecords.count(); i++)
	{
		if (ui.account->currentText() == m_accountRecords.at(i)[bankNameColumnName])
		{
			return m_accountRecords.at(i)[accountCodeColumnName];
		}
	}
	return QString();
}

void CRCheckEditDlg::setCheckCode(const QString &code)
{
	ui.code->setText(code);
}

QString CRCheckEditDlg::checkCode() const
{
	return ui.code->text();
}

void CRCheckEditDlg::setDate(const QDate &d)
{
	ui.dateEdit->setDate(d);
}

QDate CRCheckEditDlg::date() const
{
	return ui.dateEdit->date();
}

void CRCheckEditDlg::setTitle(const QString &checkTitle)
{
	ui.CRCheckTitle->setEditText(checkTitle);
}

// return the title's code
QString CRCheckEditDlg::title() const
{
	for (int i = 0; i < m_titleRecords.count(); i++)
	{
		if (ui.CRCheckTitle->currentText() == m_titleRecords.at(i)[titleMemoColumnName])
		{
			return m_titleRecords.at(i)[titleCodeColumnName];
		}
	}
	return QString();
}

void CRCheckEditDlg::setSubTitle(const QString &checkSubTitle)
{
	ui.CRCheckSubTitle->setEditText(checkSubTitle);
}

//return subtitle's code
QString CRCheckEditDlg::subTitle() const
{
	for (int i = 0; i < m_subTitleRecords.count(); i++)
	{
		if (m_subTitleRecords.at(i)[detailTitleMemoColumnName] == ui.CRCheckSubTitle->currentText())
		{
			return m_subTitleRecords.at(i)[detailTitleCodeColumnName];
		}
	}

	return QString();
}

void CRCheckEditDlg::setDepartment(const QString &checkDep)
{
	ui.department->setEditText(checkDep);
}

// return the department's code
QString CRCheckEditDlg::department() const
{
	for (int i = 0; i < m_depRecords.count(); i++)
	{
		if (m_depRecords.at(i)[depMemoColumnName] == ui.department->currentText())
		{
			return m_depRecords.at(i)[depIdColumnName];
		}
	}

	return QString();
}

void CRCheckEditDlg::setReqPeople(const QString &name)
{
	ui.reqPeople->setText(name);
}

QString CRCheckEditDlg::reqPeople() const
{
	return ui.reqPeople->text();
}

void CRCheckEditDlg::setMoney(const QString &checkMoney)
{
	ui.money->setText(checkMoney);
}

QString CRCheckEditDlg::money() const
{
	return ui.money->text();
}

void CRCheckEditDlg::setMemo(const QString &checkMemo)
{
	ui.memo->setText(checkMemo);
}

QString CRCheckEditDlg::memo() const
{
	return ui.memo->text();
}

void CRCheckEditDlg::setEditMode()
{
	ui.code->setEnabled(false);
	ui.dateEdit->setEnabled(false);
	ui.needCheck->setEnabled(false);
}
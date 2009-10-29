#include "bankeditdlg.h"
#include "databasehandler.h"

#include <QtGui>

BankEditDlg::BankEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.isCash->addItem(strYes);
	ui.isCash->addItem(strNo);
}

BankEditDlg::~BankEditDlg()
{

}

void BankEditDlg::setEditMode()
{
	ui.accountCode->setEnabled(false);
}

void BankEditDlg::setCashState(const QString &cashState)
{
	ui.isCash->setCurrentIndex((cashState==strYes)?0:1);
}

QString BankEditDlg::cashState() const
{
	return ui.isCash->currentText();
}

void BankEditDlg::setAccountCode(const QString& accountCode)
{
	ui.accountCode->setText(accountCode);
}

QString BankEditDlg::accountCode() const
{
	return ui.accountCode->text();
}

void BankEditDlg::setAccountMemo(const QString& accountMemo)
{
	ui.accountDescription->setText(accountMemo);
}

QString BankEditDlg::accountMemo() const
{
	return ui.accountDescription->text();
}

void BankEditDlg::setBankName(const QString &bankName)
{
	ui.bankName->setText(bankName);
}

QString BankEditDlg::bankName() const
{
	return ui.bankName->text();
}

void BankEditDlg::setBankAccount(const QString &bankAccount)
{
	ui.bankAccount->setText(bankAccount);
}

QString BankEditDlg::bankAccount() const
{
	return ui.bankAccount->text();
}

void BankEditDlg::accept()
{
	// TODO: data valid check
#if 1
	// check if the specified account code exist
	if (ui.accountCode->isEnabled())		// means it's insert operation, need check if the input account code is exist
	{
		QRecords records = DatabaseHandler::instance()->loadTable(BankAccountTableName);
		foreach(QArray ary, records)
		{
			if (ary[accountCodeColumnName] == accountCode())
			{
				QMessageBox::critical(this, QString::fromLocal8Bit("添加银行账号"), QString::fromLocal8Bit("指定的帐号代码已存在，请重新输入"));
				return;
			}
		}
	}
#endif
	QDialog::accept();
}
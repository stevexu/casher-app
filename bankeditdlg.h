#ifndef BANKEDITDLG_H
#define BANKEDITDLG_H

#include <QDialog>
#include "ui_bankeditdlg.h"

class BankEditDlg : public QDialog
{
	Q_OBJECT

public:
	BankEditDlg(QWidget *parent = 0);
	~BankEditDlg();

public:
	void setCashState(const QString& cashState);
	QString cashState() const;

	void setAccountCode(const QString& accountCode);
	QString accountCode() const;

	void setAccountMemo(const QString& accountMemo);
	QString accountMemo() const;

	void setBankName(const QString& bankName);
	QString bankName() const;

	void setBankAccount(const QString& bankAccount);
	QString bankAccount() const;

	void setEditMode();

public Q_SLOTS:
	virtual void accept();

private:
	Ui::BankEditDlgClass ui;
	bool m_editMode;
};

#endif // BANKEDITDLG_H

#ifndef INCOMEEDITDLG_H
#define INCOMEEDITDLG_H

#include <QDialog>
#include "ui_incomeeditdlg.h"

#ifndef QRecords
typedef QList< QMap< QString, QString > > QRecords;
#endif

class IncomeEditDlg : public QDialog
{
	Q_OBJECT

public:
	IncomeEditDlg(QWidget *parent = 0);
	~IncomeEditDlg();

public:
	void setCode(const QString& incomeCode);
	QString code() const;
	void setDate(const QDate& incomeDate);
	QDate date() const;
	void setMoney(const QString& incomeMoney);
	QString money() const;
	void setTitle(const QString& incomeTitle);
	QString title() const;
	void setMemo(const QString& incomeMemo);
	QString memo() const;
	void setAccount(const QString& incomeAccount);
	QString account() const;

	void setEditMode();

protected:
	virtual void accept();

private:
	Ui::IncomeEditDlgClass ui;
	QRecords m_titleRecords;
	QRecords m_accountRecords;
};

#endif // INCOMEEDITDLG_H

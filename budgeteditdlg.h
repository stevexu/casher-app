#ifndef BUDGETMGREDIT_H
#define BUDGETMGREDIT_H

#include <QDialog>
#include "ui_budgetmgredit.h"

class BudgetEditDlg : public QDialog
{
	Q_OBJECT

public:
	BudgetEditDlg(QWidget *parent = 0);
	~BudgetEditDlg();

public Q_SLOTS:
	virtual void accept();

public:
	void setEditMode();

	void setBudgetMemo(const QString& memo);
	QString budgetMemo() const;
	void setBudgetId(const QString& id);
	QString budgetId() const;
	void setBudgetTitle(const QString& title);
	QString budgetTitle() const;
	void setBudgetSubTitle(const QString& subTitle);
	QString budgetSubTitle() const;
	void setBudgetDate(const QDate& date);
	QDate budgetDate() const;
	void setBanlanceDownDate(const QDate& date);
	QDate balanceDownDate() const;
	void setBudget(const QString& budget);
	QString budget() const;

private:
	Ui::BudgetMgrEditClass ui;
};

#endif // BUDGETMGREDIT_H

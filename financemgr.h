#ifndef FINANCEMGR_H
#define FINANCEMGR_H

#include <QtGui>
#include "ui_financemgr.h"

class MainView;

class FinanceMgr : public QMainWindow
{
	Q_OBJECT

public:
	FinanceMgr(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FinanceMgr();

public Q_SLOTS:
	void testClick();

	void addItem();			// insert an item, call sub views method
	void editItem();		// edit an item, call sub views method
	void delItem();			// del an item, call sub views method

	// tool bar handler
	void openCalc();

	// menu handle slot
	void handleActionDepartment();					// department management
	void handleActionBank();						// bank account management
	void handleBudget();							// budget management
	void handleTitle();
	void handleDetailTitle();
	void handleIncome();
	void handleCRCheck();
	void handleMonthChanged(const QString& text);
	void hanldeCashShow();

private:
	void createToolbar();

private:
	Ui::FinanceMgrClass ui;
	MainView*  mainView;
	QDockWidget* m_docNav;
};

#endif // FINANCEMGR_H

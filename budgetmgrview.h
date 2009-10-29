#ifndef BUDGETMGRVIEW_H
#define BUDGETMGRVIEW_H

#include "tableview.h"

class BudgetMgrView : public TableView
{
	Q_OBJECT

public:
	BudgetMgrView(QWidget *parent = 0);
	~BudgetMgrView();

public:
	// add an entry with specified values
	virtual void addEntry(const QArray& item);
	// add item with a dialog
	virtual void addItem();
	// edit the selected item, if selected more than one item, just edit the first one
	virtual void editItem();
	// del the selected item, can delete more than one item
	virtual void delItem();
	// load data from the database
	virtual void loadData();

protected Q_SLOTS:
	virtual void clicked(const QModelIndex& index);
	void final();										// final the budget
	void transferToNextMonth();							// transfer the budget to next month

protected:
	virtual void createTable();

private:
	
};

#endif // BUDGETMGRVIEW_H

#ifndef INCOMEMGRVIEW_H
#define INCOMEMGRVIEW_H

#include "tableview.h"
#include "ui_incomemgrview.h"

class IncomeMgrView : public TableView
{
	Q_OBJECT

public:
	IncomeMgrView(QWidget *parent = 0);
	~IncomeMgrView();

protected:
	void createTable();

protected Q_SLOTS:
	void clicked(const QModelIndex& index);

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

private:
	QString m_lastTitleMemo;
	QString m_lastBankName;
};

#endif // INCOMEMGRVIEW_H

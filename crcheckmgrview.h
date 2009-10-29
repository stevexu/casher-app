#ifndef CRCHECKMGRVIEW_H
#define CRCHECKMGRVIEW_H

#include "tableview.h"
#include "ui_crcheckmgrview.h"

class CRCheckMgrView : public TableView
{
	Q_OBJECT

public:
	CRCheckMgrView(QWidget *parent = 0);
	~CRCheckMgrView();

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
	// here save the last insert operation's title, subtitle, account and department memo
	QString m_lastTitle;
	QString m_lastAccount;
	QString m_lastSubTitle;
	QString m_lastDepartment;
};

#endif // CRCHECKMGRVIEW_H

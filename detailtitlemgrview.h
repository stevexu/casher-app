#ifndef DETAILTITLEMGRVIEW_H
#define DETAILTITLEMGRVIEW_H

#include "tableview.h"
#include "ui_detailtitlemgrview.h"

class DetailTitleMgrView : public TableView
{
	Q_OBJECT

public:
	DetailTitleMgrView(QWidget *parent = 0);
	~DetailTitleMgrView();

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

protected:
	virtual void createTable();


private:
	
};

#endif // DETAILTITLEMGRVIEW_H

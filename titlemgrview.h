#ifndef TITLEMGRVIEW_H
#define TITLEMGRVIEW_H

#include "TableView.h"
#include "ui_titlemgrview.h"

class TitleMgrView : public TableView
{
	Q_OBJECT

public:
	TitleMgrView(QWidget *parent = 0);
	~TitleMgrView();

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

#endif // TITLEMGRVIEW_H

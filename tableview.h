#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <qtgui>
#include "tablemodel.h"
#include "ViewIdentify.h"

typedef QMap<QString, QString> QArray;

class TableView : public QWidget
{
	Q_OBJECT

public:
	TableView(QWidget *parent = 0);
	~TableView();

public:
	// add the item with specified the data
	virtual void addEntry(const QArray& item);

	// load the data from the database
	virtual void loadData();

	// get the tab name
	QString getTabName(){
		return m_tabName;
	}

	// get the view's id. department view, bank account view, e.g
	int getViewId()
	{
		return m_viewId;
	}

public Q_SLOTS:
	// add the item with dialog
	virtual void addItem();
	// edit the selected item, if selected more than one item, just edit the first selected item
	virtual void editItem();
	// del the selected item, can del more than one item
	virtual void delItem();

protected:
	virtual void resizeEvent(QResizeEvent * event);
	virtual void keyReleaseEvent( QKeyEvent * event );

protected Q_SLOTS:
	virtual void clicked(const QModelIndex& index);
	virtual void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

protected:
	// Create the grid view 
	virtual void createTable();
	virtual void setTabName(const QString& tabName){
		m_tabName = tabName;
	}
	virtual void setViewId(int nId)
	{
		m_viewId = nId;
	}

protected:
	TableModel* m_tableModel;
	QTableView* tableView;
	QString m_tabName;
	int m_viewId;
};

#endif // TABLE_VIEW_H

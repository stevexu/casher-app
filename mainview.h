#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtGui>
#include "ui_mainview.h"

class TableView;

class MainView : public QFrame
{
	Q_OBJECT

public:
	MainView(QWidget *parent = 0);
	~MainView();

public:
	void openWindow(int nWindowId);

	// add item to active table view
	void addItem();
	// edit the selected item in the active table view
	void editItem();
	// del the selected items in the active table view
	void delItem();

protected Q_SLOTS:
	void tableCloseReq(int nIndex);

protected:
	virtual void resizeEvent (QResizeEvent* event);

private:
	Ui::MainViewClass ui;
	QList<TableView*> m_createdWidgetList;
	
};

#endif // MAINVIEW_H

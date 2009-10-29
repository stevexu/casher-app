#include <QtGui>
#include "databasehandler.h"
#include "tableview.h"
#include "ui_debug.h"

TableView::TableView(QWidget *parent)
	: QWidget(parent)
{
	createTable();

	//connect(m_tableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
	//	this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));
	setContextMenuPolicy(Qt::ActionsContextMenu);
	QAction* actionNew = new QAction(QIcon(tr(":/Icon/Resources/add2.png")), QString::fromLocal8Bit("ÐÂÔö"), this);
	QAction* actionEdit = new QAction(QIcon(tr(":/Icon/Resources/edit.png")), QString::fromLocal8Bit("±à¼­"), this);
	QAction* actionDel = new QAction(QIcon(tr(":/Icon/Resources/delete.png")), QString::fromLocal8Bit("É¾³ý"), this);

	connect(actionNew, SIGNAL(triggered()), this, SLOT(addItem()));
	connect(actionEdit, SIGNAL(triggered()), this, SLOT(editItem()));
	connect(actionDel, SIGNAL(triggered()), this, SLOT(delItem()));

	addAction(actionNew);
	addAction(actionEdit);
	addAction(actionDel);
}

TableView::~TableView()
{

}

void TableView::addEntry(const QArray& item)
{
	Q_UNUSED(item)
}

void TableView::addItem()
{
	
}

void TableView::loadData()
{

}

void TableView::editItem()
{

}

void TableView::delItem()
{

}

void TableView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
	Q_UNUSED(topLeft)
	Q_UNUSED(bottomRight)
}

void TableView::keyReleaseEvent( QKeyEvent * event )
{
	Q_UNUSED(event);
}

void TableView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "TableView::createTable Entry")
	
	tableView = new QTableView(this);

	tableView->setSortingEnabled(true);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setAlternatingRowColors(true);
	
	UI_DEBUG_1(__FILE__, __LINE__, "TableView::createTable Exit")
}


void TableView::clicked(const QModelIndex& index)
{
	Q_UNUSED(index)
}

void TableView::resizeEvent(QResizeEvent * event)
{
	Q_UNUSED(event);

	tableView->setGeometry(geometry());
}
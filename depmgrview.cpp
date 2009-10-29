#include "depmgrview.h"
#include "depeditdlg.h"
#include "databasehandler.h"

#include "ui_debug.h"

DepMgrView::DepMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(DepartmentManagerView);					// set the associate id with this table
	setTabName(QString::fromLocal8Bit("部门管理"));		// set the parent table's label

	createTable();
}

DepMgrView::~DepMgrView()
{

}

void DepMgrView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::createTable Entry")
	
	m_tableModel = new TableModel(this);

	QStringList headers;
	headers << QString::fromLocal8Bit("部门编号") << QString::fromLocal8Bit("部门说明");
	m_tableModel->setHeader(headers);

	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::createTable Exit")
}

void DepMgrView::addEntry(const QArray& item)
{
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::addEntry Entry")

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());
	m_tableModel->setData(index, item[depIdColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 1, QModelIndex());
	m_tableModel->setData(index, item[depMemoColumnName], Qt::EditRole);

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::addEntry Exit")
}

void DepMgrView::addItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::addItem Entry")

	DepEditDlg dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value;
		value[depIdColumnName] = dlg.depId().toUtf8().data();
		value[depMemoColumnName] = dlg.depMemo().toUtf8().data();

		DatabaseHandler::instance()->getHandler()->insert(DepartmentTableName, value);

		// update the grid view
		QArray rowData;
		rowData[depIdColumnName] = dlg.depId();
		rowData[depMemoColumnName] = dlg.depMemo();

		addEntry(rowData);
	}

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::addItem Exit")
}

void DepMgrView::editItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::editItem Entry")
	
	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();
	if (selectedList.size() > 0)
	{
		clicked(selectedList.at(0));		// just edit the first selected item
	}
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::editItem Exit")
}

void DepMgrView::delItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::editItem Entry")
	
	int btn =  QMessageBox::question(this, 
		QString::fromLocal8Bit("删除数据"), 
		QString::fromLocal8Bit("确认删除这条数据?"),
		QMessageBox::Yes,
		QMessageBox::No);

	if (QMessageBox::Yes == btn)
	{
		QModelIndexList selectedList = tableView->selectionModel()->selectedRows();

		for (int i = 0; i < selectedList.count(); i++)
		{
			// remove it from the database
			QModelIndex index = m_tableModel->index(selectedList.at(i).row(), 0, QModelIndex());
			QString strDepId = m_tableModel->data(index, Qt::DisplayRole).toString();
 			Array row;
 			row[depIdColumnName] = strDepId.toUtf8().data();
 			DatabaseHandler::instance()->getHandler()->del(DepartmentTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
			UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::delItem delete the item")
		}
	}

	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::editItem Exit")
}

void DepMgrView::clicked(const QModelIndex& index)
{
	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::clicked Entry")

	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString itemData1 = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
	QString itemData2 = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(depIdColumnName, itemData1);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(DepartmentTableName)->where(strWhere.toUtf8().data())->get();

	std::string strId = "";
	if (dr.isValid() && dr.count() > 0)
	{
		// find the record
		Array item = (*dr.resultArray().begin());
		strId = item["id"];
	}
	else
	{
		QMessageBox::about(this, "Department Editor", "Can't find the record from the database");
		return;
	}

	DepEditDlg dlg(this);
	dlg.setData(itemData1, itemData2);
	dlg.setEditMode();
	if (dlg.exec() == QDialog::Accepted)
	{
		UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::clicked Data changed")
		selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.depId());
		selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.depMemo());

		// save the data into the database
		Array value, wheres;
		value[depIdColumnName] = dlg.depId().toUtf8().data();
		value[depMemoColumnName] = dlg.depMemo().toUtf8().data();

		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(DepartmentTableName, value, wheres);
	}

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "DepMgrView::clicked Exit")
}

void DepMgrView::loadData()
{
	// get the record from the database
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(DepartmentTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[depIdColumnName] = QString::fromUtf8(item[depIdColumnName].c_str());
			rowData[depMemoColumnName] = QString::fromUtf8(item[depMemoColumnName].c_str());
			addEntry(rowData);
		}
	}
}
#include "detailtitlemgrview.h"
#include "databasehandler.h"
#include "detailtitleeditdlg.h"
#include "ViewIdentify.h"

#include "ui_debug.h"
#include <QtGui>

DetailTitleMgrView::DetailTitleMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(DetailTitleView);
	setTabName(QString::fromLocal8Bit("详细科目管理"));

	createTable();
}

DetailTitleMgrView::~DetailTitleMgrView()
{

}

void DetailTitleMgrView::loadData()
{
	// get the record from the database
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(DetailTitleTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[detailTitleCodeColumnName] = QString::fromUtf8(item[detailTitleCodeColumnName].c_str());
			rowData[detailTitleMemoColumnName] = QString::fromUtf8(item[detailTitleMemoColumnName].c_str());
			addEntry(rowData);
		}
	}
}

void DetailTitleMgrView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::createTable Entry")

		m_tableModel = new TableModel(this);

	QStringList headers;
	headers << QString::fromLocal8Bit("详细科目编号") << 
		QString::fromLocal8Bit("详细科目描述");
	m_tableModel->setHeader(headers);

	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::createTable Exit")
}

void DetailTitleMgrView::addEntry(const QArray &item)
{
	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::addEntry Entry")

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());
	m_tableModel->setData(index, item[detailTitleCodeColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 1, QModelIndex());
	m_tableModel->setData(index, item[detailTitleMemoColumnName], Qt::EditRole);

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::addEntry Exit")
}

void DetailTitleMgrView::addItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::addItem Entry")

	DetailTitleEditDlg dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value;
		value[detailTitleCodeColumnName] = dlg.detailTitleCode().toUtf8().data();
		value[detailTitleMemoColumnName] = dlg.detailTitleMemo().toUtf8().data();

		DatabaseHandler::instance()->getHandler()->insert(DetailTitleTableName, value);

		// update the grid view
		QArray row;
		row[detailTitleCodeColumnName] = dlg.detailTitleCode();
		row[detailTitleMemoColumnName] = dlg.detailTitleMemo();
		addEntry(row);
	}

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::addItem Exit")
}

void DetailTitleMgrView::editItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::editItem Entry")

	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();
	if (selectedList.size() > 0)
	{
		clicked(selectedList.at(0));		// just edit the first selected item
	}
	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::editItem Exit")
}

void DetailTitleMgrView::delItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::editItem Entry")

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
			row[detailTitleCodeColumnName] = strDepId.toUtf8().data();
			DatabaseHandler::instance()->getHandler()->del(DetailTitleTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
			UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::delItem delete the item")
		}
	}

	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::editItem Exit")
}

void DetailTitleMgrView::clicked(const QModelIndex& index)
{
	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString detailTitleCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
	QString detailTitleMemo = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(detailTitleCodeColumnName, detailTitleCode);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(DetailTitleTableName)->where(strWhere.toUtf8().data())->get();

	std::string strId = "";
	if (dr.isValid() && dr.count() > 0)
	{
		// find the record
		Array item = (*dr.resultArray().begin());
		strId = item["id"];
	}
	else
	{
		QMessageBox::about(this, "Detail Title Editor", "Can't find the record from the database");
		return;
	}

	DetailTitleEditDlg dlg(this);
	dlg.setEditMode();
	dlg.setDetailTitleCode(detailTitleCode);
	dlg.setDetailTitleMemo(detailTitleMemo);

	if (dlg.exec() == QDialog::Accepted)
	{
		UI_DEBUG_1(__FILE__, __LINE__, "DetailTitleMgrView::clicked Data changed")
		selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.detailTitleCode());
		selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.detailTitleMemo());

		// update it in the database
		Array record, wheres;
		record[detailTitleCodeColumnName] = dlg.detailTitleCode().toUtf8().data();
		record[detailTitleMemoColumnName] = dlg.detailTitleMemo().toUtf8().data();

		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(DetailTitleTableName, record, wheres);
	}

	tableView->resizeColumnsToContents();
}
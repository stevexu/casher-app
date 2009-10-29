#include "titlemgrview.h"
#include "databasehandler.h"
#include "titleeditdlg.h"
#include "ViewIdentify.h"

#include "ui_debug.h"

TitleMgrView::TitleMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(TitleView);
	setTabName(QString::fromLocal8Bit("科目管理"));

	createTable();
}

TitleMgrView::~TitleMgrView()
{

}

void TitleMgrView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::createTable Entry")

	m_tableModel = new TableModel(this);

	QStringList headers;
	headers << QString::fromLocal8Bit("科目编号") << QString::fromLocal8Bit("科目性质") <<
		QString::fromLocal8Bit("科目描述");
	m_tableModel->setHeader(headers);

	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::createTable Exit")
}

void TitleMgrView::loadData()
{
	// get the record from the database
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(TitleTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[titleCodeColumnName] = QString::fromUtf8(item[titleCodeColumnName].c_str());
			rowData[titlePropertyColumnName] = QString::fromUtf8(item[titlePropertyColumnName].c_str());
			rowData[titleMemoColumnName] = QString::fromUtf8(item[titleMemoColumnName].c_str());
			addEntry(rowData);
		}
	}
}

void TitleMgrView::addEntry(const QArray& item)
{
	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::addEntry Entry")

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());
	m_tableModel->setData(index, item[titleCodeColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 1, QModelIndex());
	m_tableModel->setData(index, item[titlePropertyColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 2, QModelIndex());
	m_tableModel->setData(index, item[titleMemoColumnName], Qt::EditRole);

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::addEntry Exit")
}

void TitleMgrView::addItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::addItem Entry")

	TitleEditDlg dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value;
		value[titleCodeColumnName] = dlg.titleCode().toUtf8().data();
		value[titlePropertyColumnName] = dlg.titleProperty().toUtf8().data();
		value[titleMemoColumnName] = dlg.titleMemo().toUtf8().data();

		DatabaseHandler::instance()->getHandler()->insert(TitleTableName, value);

		// update the grid view
		QArray row;
		row[titleCodeColumnName] = dlg.titleCode();
		row[titlePropertyColumnName] = dlg.titleProperty();
		row[titleMemoColumnName] = dlg.titleMemo();
		addEntry(row);
	}

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::addItem Exit")
}

void TitleMgrView::editItem()
{

}

void TitleMgrView::delItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::editItem Entry")

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
			row[titleCodeColumnName] = strDepId.toUtf8().data();
			DatabaseHandler::instance()->getHandler()->del(TitleTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
			UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::delItem delete the item")
		}
	}

	UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::editItem Exit")
}

void TitleMgrView::clicked(const QModelIndex& index)
{
	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString titleCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
	QString titleProperty = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
	QString titleMemo = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(titleCodeColumnName, titleCode);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(TitleTableName)->where(strWhere.toUtf8().data())->get();

	std::string strId = "";
	if (dr.isValid() && dr.count() > 0)
	{
		// find the record
		Array item = (*dr.resultArray().begin());
		strId = item["id"];
	}
	else
	{
		QMessageBox::about(this, "Title Editor", "Can't find the record from the database");
		return;
	}

	TitleEditDlg dlg(this);
	dlg.setEditMode();
	dlg.setTitleCode(titleCode);
	dlg.setTitleProperty(titleProperty);
	dlg.setTitleMemo(titleMemo);

	if (dlg.exec() == QDialog::Accepted)
	{
		UI_DEBUG_1(__FILE__, __LINE__, "TitleMgrView::clicked Data changed")
		selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.titleCode());
		selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.titleProperty());
		selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.titleMemo());

		// update it in the database
		Array record, wheres;
		record[titleCodeColumnName] = dlg.titleCode().toUtf8().data();
		record[titlePropertyColumnName] = dlg.titleProperty().toUtf8().data();
		record[titleMemoColumnName] = dlg.titleMemo().toUtf8().data();
		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(TitleTableName, record, wheres);
	}

	tableView->resizeColumnsToContents();
}
#include "budgetmgrview.h"
#include "budgeteditdlg.h"
#include "databasehandler.h"
#include "ui_debug.h"

BudgetMgrView::BudgetMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(BudgetView);
	setTabName(QString::fromLocal8Bit("预算管理"));

	createTable();

	QAction* actionFinal = new QAction(QIcon(":/Icon/Resources/accepted_48.png"), QString::fromLocal8Bit("结案"), this);
	connect(actionFinal, SIGNAL(triggered()), this, SLOT(final()));
	addAction(actionFinal);
}

BudgetMgrView::~BudgetMgrView()
{

}

void BudgetMgrView::final()
{
	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();

	for (int i = 0; i < selectedList.count(); i++)
	{
		// final the selected items
		QModelIndex index = m_tableModel->index(selectedList.at(i).row(), 0, QModelIndex());
		QString budgetCode = m_tableModel->data(index, Qt::DisplayRole).toString();
		QString strWhere = QString("`%1` = '%2'").arg(budgetIdColumnName, budgetCode);
		
		Array updateRecord, wheres;
		updateRecord[budgetFinalStatusColumnName] = JIEAN.toUtf8().data();
		updateRecord[budgetFinalMonthColumnName] = QDate::currentDate().toString("yyyy-MM-dd").toUtf8().data();
		wheres[budgetIdColumnName] = budgetCode.toUtf8().data();
		DatabaseHandler::instance()->getHandler()->update(BudgetTableName, updateRecord, wheres);

		// update the table view display
		QModelIndex selectIndex = m_tableModel->index(index.row(), 8, QModelIndex());			// final month
		m_tableModel->setData(selectIndex, QDate::currentDate().toString("yyyy-MM-dd"));
		selectIndex = m_tableModel->index(index.row(), 9, QModelIndex());			// final status
		m_tableModel->setData(selectIndex, JIEAN);

	}

	tableView->resizeColumnsToContents();
}

void BudgetMgrView::transferToNextMonth()
{
	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();

	for (int i = 0; i < selectedList.count(); i++)
	{
		// final the selected items
		QModelIndex index = m_tableModel->index(selectedList.at(i).row(), 0, QModelIndex());
		QString budgetCode = m_tableModel->data(index, Qt::DisplayRole).toString();
		QString strWhere = QString("`%1` = '%2'").arg(budgetIdColumnName, budgetCode);

		Array updateRecord, wheres;
		updateRecord[budgetFinalStatusColumnName] = JIEAN.toUtf8().data();
		updateRecord[budgetFinalMonthColumnName] = QDate::currentDate().toString("yyyy-MM-dd").toUtf8().data();
		wheres[budgetIdColumnName] = budgetCode.toUtf8().data();
		DatabaseHandler::instance()->getHandler()->update(BudgetTableName, updateRecord, wheres);

		// update the table view display
		QModelIndex selectIndex = m_tableModel->index(index.row(), 8, QModelIndex());			// final month
		m_tableModel->setData(selectIndex, "");
		selectIndex = m_tableModel->index(index.row(), 9, QModelIndex());			// final status
		m_tableModel->setData(selectIndex, ZHUANXIAYUE);
		
	}

	tableView->resizeColumnsToContents();
}

void BudgetMgrView::clicked(const QModelIndex& index)
{
	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString budgetCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
	QString budgetMonth = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
	QString titleCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
	QString detailTitleCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 6, QModelIndex());
	QString budgetMoney = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 7, QModelIndex());
	QString budgetMemo = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	
	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(budgetIdColumnName, budgetCode);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(BudgetTableName)->where(strWhere.toUtf8().data())->get();

	std::string strId = "";
	if (dr.isValid() && dr.count() > 0)
	{
		// find the record
		Array item = (*dr.resultArray().begin());
		strId = item["id"];
	}
	else
	{
		QMessageBox::about(this, "Budget Editor", "Can't find the record from the database");
		return;
	}

	BudgetEditDlg dlg(this);
	dlg.setEditMode();
	dlg.setBudgetId(budgetCode);
	dlg.setBudgetTitle(titleCode);
	dlg.setBudgetSubTitle(detailTitleCode);
	dlg.setBudget(budgetMoney);
	dlg.setBudgetMemo(budgetMemo);

	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value, wheres;
		value[budgetIdColumnName] = dlg.budgetId().toUtf8().data();
		value[titleColumnName] = dlg.budgetTitle().toUtf8().data();
		value[subTitleColumnName] = dlg.budgetSubTitle().toUtf8().data();
		value[budgetMonthColumnName] = dlg.budgetDate().toString(QString("yyyy-MM-dd")).toUtf8().data();// convert the date to ISODate format, e.g 2009-10-20
		value[budgetColumnName] = dlg.budget().toUtf8().data();
		value[budgetMemoColumnName] = dlg.budgetMemo().toUtf8().data();

		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(BudgetTableName, value, wheres);

		// update the grid view
		QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
		QRecords detailRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);

		UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::clicked Data changed")
		selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.budgetId());
		selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.budgetDate().toString(QString("yyyy-MM-dd")));
		selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.budgetTitle());
		selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.budgetSubTitle());
		selectIndex = m_tableModel->index(clickedRow, 6, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.budget());
		selectIndex = m_tableModel->index(clickedRow, 7, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.budgetMemo());

		selectIndex = m_tableModel->index(clickedRow, 3, QModelIndex());											// budget title description
		m_tableModel->setData(selectIndex, getMemo(titleRecords, titleCodeColumnName, dlg.budgetTitle(), titleMemoColumnName), Qt::EditRole);

		selectIndex = m_tableModel->index(clickedRow, 5, QModelIndex());											// budget sub title description
		m_tableModel->setData(selectIndex, getMemo(detailRecords, detailTitleCodeColumnName, dlg.budgetSubTitle(), detailTitleMemoColumnName), Qt::EditRole);
	}

	tableView->resizeColumnsToContents();
}

void BudgetMgrView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::createTable Entry")

	m_tableModel = new TableModel(this);

	QStringList headers;
	headers << QString::fromLocal8Bit("预算编号") << 
		QString::fromLocal8Bit("预算年月") <<
		QString::fromLocal8Bit("科目") <<
		QString::fromLocal8Bit("科目说明") <<
		QString::fromLocal8Bit("明细科目") <<
		QString::fromLocal8Bit("明细说明") <<
		QString::fromLocal8Bit("预算金额") <<
		QString::fromLocal8Bit("备注") <<
		QString::fromLocal8Bit("结存年月") <<
		QString::fromLocal8Bit("结案情况");
	m_tableModel->setHeader(headers);

	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::createTable Exit")
}

/************************************************************************/
/* When modify the header, must modify the this method                  */
/************************************************************************/
void BudgetMgrView::addEntry(const QArray& item)
{
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::addEntry Entry")

	QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	QRecords detailRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());											// insert a blank row

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());								// budget code
	m_tableModel->setData(index, item[budgetIdColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 1, QModelIndex());											// budget month
	QString strDate = item[budgetCreateDateColumnName];
	QDate date = QDate::fromString(item[budgetCreateDateColumnName], Qt::ISODate);		// date from the database is ISODate format, e.g 2009-10-12
	m_tableModel->setData(index, date.isNull()?QString(""):date.toString(Qt::ISODate), Qt::EditRole);	

	index = m_tableModel->index(row, 2, QModelIndex());											// budget title
	m_tableModel->setData(index, item[titleColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 3, QModelIndex());											// budget title description
	m_tableModel->setData(index, getMemo(titleRecords, titleCodeColumnName, item[titleColumnName], titleMemoColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 4, QModelIndex());											// budget sub title
	m_tableModel->setData(index, item[subTitleColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 5, QModelIndex());											// budget sub title description
	m_tableModel->setData(index, getMemo(detailRecords, detailTitleCodeColumnName, item[subTitleColumnName], detailTitleMemoColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 6, QModelIndex());											// budget money
	m_tableModel->setData(index, item[budgetColumnName], Qt::EditRole);
	
	index = m_tableModel->index(row, 7, QModelIndex());											// budget memo
	m_tableModel->setData(index, item[budgetMemoColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 8, QModelIndex());											// budget final month
	QDate dateBalanceDown = QDate::fromString(item[budgetFinalMonthColumnName], Qt::ISODate);
	m_tableModel->setData(index, dateBalanceDown.isNull()?"":dateBalanceDown.toString(Qt::ISODate), Qt::EditRole);

	index = m_tableModel->index(row, 9, QModelIndex());											// budget final status
	m_tableModel->setData(index, item[budgetFinalStatusColumnName], Qt::EditRole);
	
	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::addEntry Exit")
}

void BudgetMgrView::addItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::addItem Entry")

	BudgetEditDlg dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value;
		value[budgetIdColumnName] = dlg.budgetId().toUtf8().data();
		value[titleColumnName] = dlg.budgetTitle().toUtf8().data();
		value[subTitleColumnName] = dlg.budgetSubTitle().toUtf8().data();
		value[budgetMonthColumnName] = dlg.budgetDate().toString(QString("yyyy-MM-dd")).toUtf8().data();// convert the date to ISODate format, e.g 2009-10-20
		value[budgetColumnName] = dlg.budget().toUtf8().data();
		value[budgetMemoColumnName] = dlg.budgetMemo().toUtf8().data();
		value[budgetFinalStatusColumnName] = WEIJIE.toUtf8().data();
		value[budgetCreateDateColumnName] = value[budgetMonthColumnName];

		DatabaseHandler::instance()->getHandler()->insert(BudgetTableName, value);

		// update the grid view
		QArray row;
		row[budgetIdColumnName] = dlg.budgetId();
		row[budgetCreateDateColumnName] = dlg.budgetDate().toString(QString("yyyy-MM-dd")); // convert date format
		row[titleColumnName] = dlg.budgetTitle();
		row[subTitleColumnName] = dlg.budgetSubTitle();
		row[budgetFinalMonthColumnName] = dlg.balanceDownDate().toString();
		row[budgetMemoColumnName] = dlg.budgetMemo();
		row[budgetColumnName] = dlg.budget();
		row[budgetFinalStatusColumnName] = WEIJIE;

		addEntry(row);
	}

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::addItem Exit")
}

void BudgetMgrView::editItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::editItem Entry")

	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();
	if (selectedList.size() > 0)
	{
		clicked(selectedList.at(0));		// just edit the first selected item
	}
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::editItem Exit")
}

void BudgetMgrView::delItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::delItem Entry")

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
			row[budgetIdColumnName] = strDepId.toUtf8().data();
			DatabaseHandler::instance()->getHandler()->del(BudgetTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
			UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::delItem delete the item")
		}
	}

	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::delItem Exit")
}

void BudgetMgrView::loadData()
{
	// get the record from the database
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(BudgetTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[budgetIdColumnName] = QString::fromUtf8(item[budgetIdColumnName].c_str());
			rowData[titleColumnName] = QString::fromUtf8(item[titleColumnName].c_str());
			rowData[subTitleColumnName] = QString::fromUtf8(item[subTitleColumnName].c_str());
			rowData[budgetCreateDateColumnName] = QString::fromUtf8(item[budgetCreateDateColumnName].c_str());
			rowData[budgetColumnName] = QString::fromUtf8(item[budgetColumnName].c_str());
			rowData[budgetFinalMonthColumnName] = QString::fromUtf8(item[budgetFinalMonthColumnName].c_str());
			rowData[budgetMemoColumnName] = QString::fromUtf8(item[budgetMemoColumnName].c_str());
			rowData[budgetFinalStatusColumnName] = QString::fromUtf8(item[budgetFinalStatusColumnName].c_str());
			addEntry(rowData);
		}
	}
}
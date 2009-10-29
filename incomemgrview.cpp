#include "incomemgrview.h"
#include "ui_debug.h"
#include "databasehandler.h"
#include "incomeeditdlg.h"

IncomeMgrView::IncomeMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(IncomeView);
	setTabName(QString::fromLocal8Bit("收入管理"));

	createTable();

}

IncomeMgrView::~IncomeMgrView()
{

}

void IncomeMgrView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::createTable Entry")

	m_tableModel = new TableModel(this);

	QStringList headers;
	headers << QString::fromLocal8Bit("收入单号") << 
		QString::fromLocal8Bit("收入日期") <<
		QString::fromLocal8Bit("收入帐号") <<
		QString::fromLocal8Bit("科目") <<
		QString::fromLocal8Bit("收入金额") <<
		QString::fromLocal8Bit("摘要");

	m_tableModel->setHeader(headers);

	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	UI_DEBUG_1(__FILE__, __LINE__, "BudgetMgrView::createTable Exit")
}

void IncomeMgrView::loadData()
{
	// get the record from the database
	//QRecords titles = DatabaseHandler::instance()->loadTable(TitleTableName);
	//QRecords subTitles = DatabaseHandler::instance()->loadTable(DetailTitleTableName);

	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(IncomeTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[incomeCodeColumnName] = QString::fromUtf8(item[incomeCodeColumnName].c_str());
			rowData[incomeDateColumnName] = QString::fromUtf8(item[incomeDateColumnName].c_str());
			rowData[incomeAccountColumnName] = QString::fromUtf8(item[incomeAccountColumnName].c_str());
			rowData[incomeTitleColumnName] = QString::fromUtf8(item[incomeTitleColumnName].c_str());
			rowData[incomeMoneyColumnName] = QString::fromUtf8(item[incomeMoneyColumnName].c_str());
			rowData[incomeMemoColumnName] = QString::fromUtf8(item[incomeMemoColumnName].c_str());
			addEntry(rowData);
		}
	}
}

void IncomeMgrView::addEntry(const QArray &item)
{
	UI_DEBUG_1(__FILE__, __LINE__, "IncomeMgrView::addEntry Entry")

	QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	QRecords accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());											// insert a blank row

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());								// income code
	m_tableModel->setData(index, item[incomeCodeColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 1, QModelIndex());											// income date
	QString strDate = item[incomeDateColumnName];
	QDate date = QDate::fromString(strDate, Qt::ISODate);		// date from the database is ISODate format, e.g 2009-10-12
	m_tableModel->setData(index, date.isNull()?QString(""):date.toString(Qt::ISODate), Qt::EditRole);	

	index = m_tableModel->index(row, 2, QModelIndex());											// income account
	m_tableModel->setData(index, getMemo(accountRecords, accountCodeColumnName, item[incomeAccountColumnName], bankNameColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 3, QModelIndex());											// income title
	m_tableModel->setData(index, getMemo(titleRecords, titleCodeColumnName, item[incomeTitleColumnName], titleMemoColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 4, QModelIndex());											// income money
	m_tableModel->setData(index, item[incomeMoneyColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 5, QModelIndex());											// income memo
	m_tableModel->setData(index, item[incomeMemoColumnName], Qt::EditRole);

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "IncomeMgrView::addEntry Exit")
}

void IncomeMgrView::addItem()
{
	IncomeEditDlg dlg(this);
	dlg.setAccount(m_lastBankName);
	dlg.setTitle(m_lastTitleMemo);

	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value;
		value[incomeCodeColumnName] = dlg.code().toUtf8().data();
		value[incomeDateColumnName] = dlg.date().toString(QString("yyyy-MM-dd")).toUtf8().data();
		value[incomeMoneyColumnName] = dlg.money().toUtf8().data();
		value[incomeTitleColumnName] = dlg.title().toUtf8().data();
		value[incomeAccountColumnName] = dlg.account().toUtf8().data();
		value[incomeMemoColumnName] = dlg.memo().toUtf8().data();

		DatabaseHandler::instance()->getHandler()->insert(IncomeTableName, value);

		// update the grid view
		QArray row;
		row[incomeCodeColumnName] = dlg.code();
		row[incomeDateColumnName] = dlg.date().toString(QString("yyyy-MM-dd"));
		row[incomeMoneyColumnName] = dlg.money();
		row[incomeTitleColumnName] = dlg.title();
		row[incomeAccountColumnName] = dlg.account();
		row[incomeMemoColumnName] = dlg.memo();

		QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
		QRecords accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);

		m_lastTitleMemo = getMemo(titleRecords, titleCodeColumnName, dlg.title(), titleMemoColumnName);
		m_lastBankName = getMemo(accountRecords, accountCodeColumnName, dlg.account(), bankNameColumnName);

		addEntry(row);
	}

	tableView->resizeColumnsToContents();
}

void IncomeMgrView::editItem()
{
	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();
	if (selectedList.size() > 0)
	{
		clicked(selectedList.at(0));		// just edit the first selected item
	}
}

void IncomeMgrView::delItem()
{
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
			row[incomeCodeColumnName] = strDepId.toUtf8().data();
			DatabaseHandler::instance()->getHandler()->del(IncomeTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
		}
	}
}

void IncomeMgrView::clicked(const QModelIndex& index)
{
	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString incomeCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
	QString incomeDate = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
	QString incomeAccount = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	selectIndex = m_tableModel->index(clickedRow, 3, QModelIndex());
	QString incomeTitle = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
	QString incomeMoney = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	selectIndex = m_tableModel->index(clickedRow, 5, QModelIndex());
	QString incomeMemo = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();


	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(incomeCodeColumnName, incomeCode);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(IncomeTableName)->where(strWhere.toUtf8().data())->get();

	std::string strId = "";
	if (dr.isValid() && dr.count() > 0)
	{
		// find the record
		Array item = (*dr.resultArray().begin());
		strId = item["id"];
	}
	else
	{
		QMessageBox::about(this, "Income Editor", "Can't find the record from the database");
		return;
	}

	QRecords incomeRecords = DatabaseHandler::instance()->loadTable(IncomeTableName);

	IncomeEditDlg dlg(this);
	dlg.setEditMode();
	dlg.setCode(incomeCode);
	dlg.setDate(QDate::fromString(incomeDate, Qt::ISODate));
	dlg.setMoney(incomeMoney);
	// here to use title code and subtitle code for edit, displays are their memo
	dlg.setTitle(incomeTitle);
	dlg.setAccount(incomeAccount);
	dlg.setMemo(incomeMemo);

	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value, wheres;
		value[incomeCodeColumnName] = dlg.code().toUtf8().data();
		value[incomeDateColumnName] = dlg.date().toString(QString("yyyy-MM-dd")).toUtf8().data();
		value[incomeMoneyColumnName] = dlg.money().toUtf8().data();
		value[incomeTitleColumnName] = dlg.title().toUtf8().data();
		value[incomeAccountColumnName] = dlg.account().toUtf8().data();
		value[incomeMemoColumnName] = dlg.memo().toUtf8().data();

		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(IncomeTableName, value, wheres);

		// update the grid view
		QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
		QRecords detailRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
		QRecords accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);

		selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.code());

		selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.date().toString(QString("yyyy-MM-dd")));

		selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
		m_tableModel->setData(selectIndex, getMemo(accountRecords, accountCodeColumnName, dlg.account(), bankNameColumnName));

		selectIndex = m_tableModel->index(clickedRow, 3, QModelIndex());
		m_tableModel->setData(selectIndex, getMemo(titleRecords, titleCodeColumnName, dlg.title(), titleMemoColumnName));
		
		selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.money());

		selectIndex = m_tableModel->index(clickedRow, 5, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.memo());
	}

	tableView->resizeColumnsToContents();
}
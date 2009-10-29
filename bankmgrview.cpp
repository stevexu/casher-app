#include "bankmgrview.h"
#include "databasehandler.h"
#include "bankeditdlg.h"
#include "ui_debug.h"

BankMgrView::BankMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(BankAccountView);
	setTabName(QString::fromLocal8Bit("帐户管理"));

	createTable();
}

BankMgrView::~BankMgrView()
{

}

void BankMgrView::createTable()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::createTable Entry")

	m_tableModel = new TableModel(this);
	
	QStringList headers;
	headers << QString::fromLocal8Bit("帐户代码") << QString::fromLocal8Bit("帐户描述") <<
		QString::fromLocal8Bit("开户行名称") <<
		QString::fromLocal8Bit("开户行帐号") <<
		QString::fromLocal8Bit("现金帐户");
	m_tableModel->setHeader(headers);
	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::createTable Exit")
}

void BankMgrView::loadData()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::loadData Entry")

	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(BankAccountTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[accountCodeColumnName] = QString::fromUtf8(item[accountCodeColumnName].c_str());
			rowData[accountDescriptionColumnName] = QString::fromUtf8(item[accountDescriptionColumnName].c_str());
			rowData[bankNameColumnName] = QString::fromUtf8(item[bankNameColumnName].c_str());
			rowData[bankAccountColumnName] = QString::fromUtf8(item[bankAccountColumnName].c_str());
			rowData[isCashColumnName] = QString::fromUtf8(item[isCashColumnName].c_str());
			addEntry(rowData);
		}
	}

	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::loadData Exit")
}

void BankMgrView::addEntry(const QArray &item)
{
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::addEntry Entry")

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());
	m_tableModel->setData(index, item[accountCodeColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 1, QModelIndex());
	m_tableModel->setData(index, item[accountDescriptionColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 2, QModelIndex());
	m_tableModel->setData(index, item[bankNameColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 3, QModelIndex());
	m_tableModel->setData(index, item[bankAccountColumnName], Qt::EditRole);
	index = m_tableModel->index(row, 4, QModelIndex());
	m_tableModel->setData(index, (item[isCashColumnName].toUInt() > 0)?strYes:strNo, Qt::EditRole);

	tableView->resizeColumnsToContents();

	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::addEntry Exit")	
}

void BankMgrView::addItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::addItem Entry")

	BankEditDlg dlg(this);

	if (dlg.exec() == QDialog::Accepted)
	{
		QArray row;
		row[accountCodeColumnName] = dlg.accountCode();
		row[accountDescriptionColumnName] = dlg.accountMemo();
		row[bankNameColumnName] = dlg.bankName();
		row[bankAccountColumnName] = dlg.bankAccount();
		row[isCashColumnName] = (dlg.cashState() == strYes)?"1":"0";

		addEntry(row);

		// add it in the database
		Array record;
		record[accountCodeColumnName] = dlg.accountCode().toUtf8().data();
		record[accountDescriptionColumnName] = dlg.accountMemo().toUtf8().data();
		record[bankNameColumnName] = dlg.bankName().toUtf8().data();
		record[bankAccountColumnName] = dlg.bankAccount().toUtf8().data();
		record[isCashColumnName] = (dlg.cashState() == strYes)?"1":"0";
		DatabaseHandler::instance()->getHandler()->insert(BankAccountTableName, record);
	}

	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::addItem Exit")
}

void BankMgrView::editItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::editItem Entry")

	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();
	if (selectedList.size() > 0)
	{
		clicked(selectedList.at(0));		// just edit the first selected item
	}
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::editItem Exit")
}

void BankMgrView::delItem()
{
	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::editItem Entry")

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
			QString id = m_tableModel->data(index, Qt::DisplayRole).toString();
			Array row;
			row[accountCodeColumnName] = id.toUtf8().data();
			DatabaseHandler::instance()->getHandler()->del(BankAccountTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
			UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::delItem delete the item")
		}
	}

	UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::editItem Exit")
}

void BankMgrView::clicked(const QModelIndex& index)
{
	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString accountCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
	QString accountMemo = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
	QString bankName = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 3, QModelIndex());
	QString bankAccount = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();
	selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
	QString cashState = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(accountCodeColumnName, accountCode);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(BankAccountTableName)->where(strWhere.toUtf8().data())->get();

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

	BankEditDlg dlg(this);
	dlg.setEditMode();
	dlg.setAccountCode(accountCode);
	dlg.setAccountMemo(accountMemo);
	dlg.setBankName(bankName);
	dlg.setBankAccount(bankAccount);
	dlg.setCashState(cashState);

	if (dlg.exec() == QDialog::Accepted)
	{
		UI_DEBUG_1(__FILE__, __LINE__, "BankMgrView::clicked Data changed")
		selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.accountCode());
		selectIndex = m_tableModel->index(clickedRow, 1, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.accountMemo());
		selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.bankName());
		selectIndex = m_tableModel->index(clickedRow, 3, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.bankAccount());
		selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.cashState());

		// update it in the database
		Array record, wheres;
		record[accountCodeColumnName] = dlg.accountCode().toUtf8().data();
		record[accountDescriptionColumnName] = dlg.accountMemo().toUtf8().data();
		record[bankNameColumnName] = dlg.bankName().toUtf8().data();
		record[bankAccountColumnName] = dlg.bankAccount().toUtf8().data();
		record[isCashColumnName] = (dlg.cashState() == strYes)?"1":"0";
		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(BankAccountTableName, record, wheres);
	}

	tableView->resizeColumnsToContents();
}
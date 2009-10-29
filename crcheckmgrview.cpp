#include "ui_debug.h"
#include "databasehandler.h"
#include "crcheckeditdlg.h"
#include "crcheckmgrview.h"

CRCheckMgrView::CRCheckMgrView(QWidget *parent)
	: TableView(parent)
{
	setViewId(CRCheckView);
	setTabName(QString::fromLocal8Bit("请款单管理"));

	createTable();
}

CRCheckMgrView::~CRCheckMgrView()
{

}

void CRCheckMgrView::createTable()
{
	m_tableModel = new TableModel(this);

	QStringList headers;
	headers << QString::fromLocal8Bit("请款单编号") << 
		QString::fromLocal8Bit("申请日期") <<
		QString::fromLocal8Bit("部门") <<
		QString::fromLocal8Bit("部门说明") <<
		QString::fromLocal8Bit("科目") <<
		QString::fromLocal8Bit("科目说明") <<
		QString::fromLocal8Bit("详细科目") <<
		QString::fromLocal8Bit("详细科目说明") <<
		QString::fromLocal8Bit("摘要") <<
		QString::fromLocal8Bit("金额") <<
		QString::fromLocal8Bit("结存日期");


	m_tableModel->setHeader(headers);

	// very important, model must created before the view created
	TableView::createTable();

	tableView->setModel(m_tableModel);
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));
}

void CRCheckMgrView::loadData()
{
	// get the record from the database
	//QRecords titles = DatabaseHandler::instance()->loadTable(TitleTableName);
	//QRecords subTitles = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
	//QRecords departments = DatabaseHandler::instance()->loadTable(DepartmentTableName);

	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(CRCheckTableName)->get();
	if (dr.count() > 0)
	{
		ResultArray& rAry = dr.resultArray();
		foreach(Array item, rAry)
		{
			QArray rowData;
			rowData[checkCodeColumnName] = QString::fromUtf8(item[checkCodeColumnName].c_str());
			rowData[checkDepCodeColumnName] = QString::fromUtf8(item[checkDepCodeColumnName].c_str());
			rowData[checkTitleCodeColumnName] = QString::fromUtf8(item[checkTitleCodeColumnName].c_str());
			rowData[checkSubTitleCodeColumnName] = QString::fromUtf8(item[checkSubTitleCodeColumnName].c_str());
			rowData[checkMemoColumnName] = QString::fromUtf8(item[checkMemoColumnName].c_str());
			rowData[checkReqMoneyColumnName] = QString::fromUtf8(item[checkReqMoneyColumnName].c_str());
			rowData[checkReqDateColumnName] = QString::fromUtf8(item[checkReqDateColumnName].c_str());
			rowData[checkStrikeDateColumnName] = QString::fromUtf8(item[checkStrikeDateColumnName].c_str());
			addEntry(rowData);
		}
	}
}

void CRCheckMgrView::addEntry(const QArray &item)
{
	QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	QRecords subTitleRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
	QRecords departmentRecords = DatabaseHandler::instance()->loadTable(DepartmentTableName);

	int row = 0;
	m_tableModel->insertRows(row, 1, QModelIndex());											// insert a blank row

	QModelIndex index = m_tableModel->index(row, 0, QModelIndex());								// check code
	m_tableModel->setData(index, item[checkCodeColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 1, QModelIndex());											// check date
	QString strDate = item[checkReqDateColumnName];
	QDate date = QDate::fromString(strDate, Qt::ISODate);		// date from the database is ISODate format, e.g 2009-10-12
	m_tableModel->setData(index, date.isNull()?QString(""):date.toString(Qt::ISODate), Qt::EditRole);	

	index = m_tableModel->index(row, 2, QModelIndex());											// require department
	m_tableModel->setData(index, item[checkDepCodeColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 3, QModelIndex());											// require department memo
	m_tableModel->setData(index, getMemo(departmentRecords, depIdColumnName, item[checkDepCodeColumnName], depMemoColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 4, QModelIndex());											// require title
	m_tableModel->setData(index, item[checkTitleCodeColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 5, QModelIndex());											// income title
	m_tableModel->setData(index, getMemo(titleRecords, titleCodeColumnName, item[checkTitleCodeColumnName], titleMemoColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 6, QModelIndex());											// require sub title
	m_tableModel->setData(index, item[checkSubTitleCodeColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 7, QModelIndex());											// require sub title memo
	m_tableModel->setData(index, getMemo(subTitleRecords, detailTitleCodeColumnName, item[checkSubTitleCodeColumnName], detailTitleMemoColumnName), Qt::EditRole);

	index = m_tableModel->index(row, 8, QModelIndex());											// check memo
	m_tableModel->setData(index, item[checkMemoColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 9, QModelIndex());											// check money
	m_tableModel->setData(index, item[checkReqMoneyColumnName], Qt::EditRole);

	index = m_tableModel->index(row, 10, QModelIndex());											// check strike date
	QDate strikeDate = QDate::fromString(item[checkStrikeDateColumnName], Qt::ISODate);
	m_tableModel->setData(index, strikeDate.isNull()?QString(""):strikeDate.toString(Qt::ISODate), Qt::EditRole);

	tableView->resizeColumnsToContents();
}

void CRCheckMgrView::clicked(const QModelIndex& index)
{
	int clickedRow = index.row();
	QModelIndex selectIndex = m_tableModel->index(clickedRow, 0, QModelIndex());
	QString checkCode = m_tableModel->data(selectIndex, Qt::DisplayRole).toString();

	// find the record in the database by depId
	QString strWhere = QString("`%1` = '%2'").arg(checkCodeColumnName, checkCode);
	Database_Result& dr = DatabaseHandler::instance()->getHandler()->from(CRCheckTableName)->where(strWhere.toUtf8().data())->get();

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
	
	Array record = *dr.resultArray().begin();

	QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
	QRecords subTitleRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
	QRecords accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);
	QRecords departmentRecords = DatabaseHandler::instance()->loadTable(DepartmentTableName);

	QString titleMemo = getMemo(titleRecords, titleCodeColumnName, record[checkTitleCodeColumnName].c_str(), titleMemoColumnName);
	QString bankName = getMemo(accountRecords, accountCodeColumnName, record[checkAccountCodeColumnName].c_str(), bankNameColumnName);
	QString subTitle = getMemo(subTitleRecords, detailTitleCodeColumnName, record[checkSubTitleCodeColumnName].c_str(), detailTitleMemoColumnName);
	QString departmentMemo = getMemo(departmentRecords, depIdColumnName, record[checkDepCodeColumnName].c_str(), depMemoColumnName);

	CRCheckEditDlg dlg(this);
	dlg.setEditMode();
	dlg.setCheckCode(record[checkCodeColumnName].c_str());
	dlg.setDate(QDate::fromString(record[checkReqDateColumnName].c_str(), Qt::ISODate));
	dlg.setMoney(record[checkReqMoneyColumnName].c_str());
	// here to use title code and subtitle code for edit, displays are their memo
	dlg.setTitle(titleMemo);
	dlg.setAccount(bankName);
	dlg.setMemo(record[checkMemoColumnName].c_str());
	dlg.setDepartment(departmentMemo);
	dlg.setSubTitle(subTitle);
	dlg.setReqPeople(record[checkReqPeopleColumnName].c_str());

	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value, wheres;
		value[checkTitleCodeColumnName] = dlg.title().toUtf8().data();
		value[checkSubTitleCodeColumnName] = dlg.subTitle().toUtf8().data();
		value[checkDepCodeColumnName] = dlg.department().toUtf8().data();
		value[checkMemoColumnName] = dlg.memo().toUtf8().data();
		value[checkReqPeopleColumnName] = dlg.reqPeople().toUtf8().data();
		value[checkReqMoneyColumnName] = dlg.money().toUtf8().data();
		value[checkAccountCodeColumnName] = dlg.account().toUtf8().data();

		wheres["id"] = strId;

		bool success = DatabaseHandler::instance()->getHandler()->update(CRCheckTableName, value, wheres);

		// update the grid view
		selectIndex = m_tableModel->index(clickedRow, 2, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.department());

		selectIndex = m_tableModel->index(clickedRow, 3, QModelIndex());
		m_tableModel->setData(selectIndex, getMemo(departmentRecords, depIdColumnName, dlg.department(), depMemoColumnName));

		selectIndex = m_tableModel->index(clickedRow, 4, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.title());

		selectIndex = m_tableModel->index(clickedRow, 5, QModelIndex());
		m_tableModel->setData(selectIndex, getMemo(titleRecords, titleCodeColumnName, dlg.title(), titleMemoColumnName));

		selectIndex = m_tableModel->index(clickedRow, 6, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.subTitle());

		selectIndex = m_tableModel->index(clickedRow, 7, QModelIndex());
		m_tableModel->setData(selectIndex, getMemo(subTitleRecords, detailTitleCodeColumnName, dlg.subTitle(), detailTitleMemoColumnName));

		selectIndex = m_tableModel->index(clickedRow, 8, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.memo());

		selectIndex = m_tableModel->index(clickedRow, 9, QModelIndex());
		m_tableModel->setData(selectIndex, dlg.money());

	}

	tableView->resizeColumnsToContents();
}

void CRCheckMgrView::addItem()
{
	CRCheckEditDlg dlg(this);
	dlg.setTitle(m_lastTitle);
	dlg.setSubTitle(m_lastSubTitle);
	dlg.setAccount(m_lastAccount);
	dlg.setDepartment(m_lastDepartment);
	
	if (dlg.exec() == QDialog::Accepted)
	{
		// save the data into the database
		Array value;
		value[checkCodeColumnName] = dlg.checkCode().toUtf8().data();
		value[checkReqDateColumnName] = dlg.date().toString(QString("yyyy-MM-dd")).toUtf8().data();
		value[checkTitleCodeColumnName] = dlg.title().toUtf8().data();
		value[checkSubTitleCodeColumnName] = dlg.subTitle().toUtf8().data();
		value[checkDepCodeColumnName] = dlg.department().toUtf8().data();
		value[checkMemoColumnName] = dlg.memo().toUtf8().data();
		value[checkReqPeopleColumnName] = dlg.reqPeople().toUtf8().data();
		value[checkReqMoneyColumnName] = dlg.money().toUtf8().data();
		value[checkAccountCodeColumnName] = dlg.account().toUtf8().data();

		DatabaseHandler::instance()->getHandler()->insert(CRCheckTableName, value);

		// update the grid view
		QArray row;
		row[checkCodeColumnName] = dlg.checkCode();
		row[checkReqDateColumnName] = dlg.date().toString(QString("yyyy-MM-dd"));
		row[checkTitleCodeColumnName] = dlg.title();
		row[checkSubTitleCodeColumnName] = dlg.subTitle();
		row[checkDepCodeColumnName] = dlg.department();
		row[checkMemoColumnName] = dlg.memo();
		row[checkReqPeopleColumnName] = dlg.reqPeople();
		row[checkReqMoneyColumnName] = dlg.money();
		row[checkAccountCodeColumnName] = dlg.account();

		addEntry(row);

		QRecords titleRecords = DatabaseHandler::instance()->loadTable(TitleTableName);
		QRecords subTitleRecords = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
		QRecords accountRecords = DatabaseHandler::instance()->loadTable(BankAccountTableName);
		QRecords departmentRecords = DatabaseHandler::instance()->loadTable(DepartmentTableName);

		m_lastTitle = getMemo(titleRecords, titleCodeColumnName, dlg.title(), titleMemoColumnName);
		m_lastAccount = getMemo(accountRecords, accountCodeColumnName, dlg.account(), bankNameColumnName);
		m_lastSubTitle = getMemo(subTitleRecords, detailTitleCodeColumnName, dlg.subTitle(), detailTitleMemoColumnName);
		m_lastDepartment = getMemo(departmentRecords, depIdColumnName, dlg.department(), depMemoColumnName);

	}

	tableView->resizeColumnsToContents();
}

void CRCheckMgrView::editItem()
{
	QModelIndexList selectedList = tableView->selectionModel()->selectedRows();
	if (selectedList.size() > 0)
	{
		clicked(selectedList.at(0));		// just edit the first selected item
	}
}

void CRCheckMgrView::delItem()
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
			row[checkCodeColumnName] = strDepId.toUtf8().data();
			DatabaseHandler::instance()->getHandler()->del(CRCheckTableName, row);

			// remove it from the grid view
			m_tableModel->removeRow(selectedList[i].row());
		}
	}
}
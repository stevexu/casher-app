#include "depeditdlg.h"
#include "databasehandler.h"
#include <QtGui>

DepEditDlg::DepEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

DepEditDlg::~DepEditDlg()
{

}

void DepEditDlg::setEditMode()
{
	ui.depId_editor->setEnabled(false);
}

void DepEditDlg::setData(const QString& depId, const QString& depMemo)
{
	ui.depId_editor->insert(depId);
	ui.depMemo_editor->insert(depMemo);
}

QString DepEditDlg::depId() const
{
	return ui.depId_editor->text();
}

QString DepEditDlg::depMemo() const{
	return ui.depMemo_editor->text();
}

void DepEditDlg::accept()
{
	// check the data if it's legal
	foreach(QChar ch, ui.depId_editor->text())
	{
		if ((ch >= QChar('0') && ch <= QChar('9')) || (ch.toLower() >= QChar('a') && ch.toLower() <= QChar('z')) )
		{
			continue;
		}

		QMessageBox::about(this, QString::fromLocal8Bit("部门编辑"), QString::fromLocal8Bit("部门编号只能是8位英文或者数字"));
		return;
	}

	if (ui.depId_editor->text().size() > 8)
	{
		QMessageBox::about(this, QString::fromLocal8Bit("部门编辑"), QString::fromLocal8Bit("部门编号只能是英文或者数字"));
		return;
	}

	if (ui.depId_editor->text().toUtf8().size() > 255)
	{
		QMessageBox::about(this, QString::fromLocal8Bit("部门编辑"), QString::fromLocal8Bit("部门描述超过长度"));
		return;
	}

	// here to check if the input department code is exist
	if (ui.depId_editor->isEnabled())		// means insert operation, need check if the input department code is exist
	{
		QRecords records = DatabaseHandler::instance()->loadTable(DepartmentTableName);
		foreach(QArray ary, records)
		{
			if (ary[depIdColumnName] == depId())
			{
				QMessageBox::critical(this, QString::fromLocal8Bit("添加部门"), QString::fromLocal8Bit("指定的部门代码已存在，请重新输入"));
				return;
			}
		}
	}
	QDialog::accept();
}
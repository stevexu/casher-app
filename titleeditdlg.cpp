#include "titleeditdlg.h"
#include "databasehandler.h"

#include <qtgui>

TitleEditDlg::TitleEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

TitleEditDlg::~TitleEditDlg()
{

}

void TitleEditDlg::setEditMode()
{
	ui.titleCode->setEnabled(false);
}

void TitleEditDlg::setTitleCode(const QString& code)
{
	ui.titleCode->setText(code);
}

QString TitleEditDlg::titleCode() const{
	return ui.titleCode->text();
}

void TitleEditDlg::setTitleProperty(const QString& property)
{
	ui.titleProperty->setCurrentIndex((property == JIE)?0:1);
}

QString TitleEditDlg::titleProperty() const
{
	return ui.titleProperty->currentText();
}

void TitleEditDlg::setTitleMemo(const QString &memo)
{
	ui.titleMemo->setText(memo);
}

QString TitleEditDlg::titleMemo() const
{
	return ui.titleMemo->text();
}

void TitleEditDlg::accept()
{
	// here to check if the input department code is exist
	if (ui.titleCode->isEnabled())		// means insert operation, need check if the input department code is exist
	{
		QRecords records = DatabaseHandler::instance()->loadTable(TitleTableName);
		foreach(QArray ary, records)
		{
			if (ary[titleCodeColumnName] == titleCode())
			{
				QMessageBox::critical(this, QString::fromLocal8Bit("添加科目"), QString::fromLocal8Bit("指定的科目代码已存在，请重新输入"));
				return;
			}
		}
	}

	QDialog::accept();
}
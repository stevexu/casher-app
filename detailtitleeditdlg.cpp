#include "detailtitleeditdlg.h"
#include "databasehandler.h"

#include <QtGui>

DetailTitleEditDlg::DetailTitleEditDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

DetailTitleEditDlg::~DetailTitleEditDlg()
{

}

void DetailTitleEditDlg::setEditMode()
{
	ui.detailTitleCode->setEnabled(false);
}

void DetailTitleEditDlg::setDetailTitleCode(const QString &code)
{
	ui.detailTitleCode->setText(code);
}

QString DetailTitleEditDlg::detailTitleCode() const
{
	return ui.detailTitleCode->text();
}

void DetailTitleEditDlg::setDetailTitleMemo(const QString &memo)
{
	ui.detailTitleMemo->setText(memo);
}

QString DetailTitleEditDlg::detailTitleMemo() const
{
	return ui.detailTitleMemo->text();
}


void DetailTitleEditDlg::accept()
{
	// here to check if the input department code is exist
	if (ui.detailTitleCode->isEnabled())		// means insert operation, need check if the input department code is exist
	{
		QRecords records = DatabaseHandler::instance()->loadTable(DetailTitleTableName);
		foreach(QArray ary, records)
		{
			if (ary[detailTitleCodeColumnName] == detailTitleCode())
			{
				QMessageBox::critical(this, QString::fromLocal8Bit("添加详细科目"), QString::fromLocal8Bit("指定的详细科目代码已存在，请重新输入"));
				return;
			}
		}
	}
	QDialog::accept();
}
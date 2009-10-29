#ifndef DETAILTITLEEDITDLG_H
#define DETAILTITLEEDITDLG_H

#include <QDialog>
#include "ui_detailtitleeditdlg.h"

class DetailTitleEditDlg : public QDialog
{
	Q_OBJECT

public:
	DetailTitleEditDlg(QWidget *parent = 0);
	~DetailTitleEditDlg();

public Q_SLOTS:
	virtual void accept();

public:
	void setDetailTitleCode(const QString& code);
	QString detailTitleCode() const;
	void setDetailTitleMemo(const QString& memo);
	QString detailTitleMemo() const;
	void setEditMode();

private:
	Ui::DetailTitleEditDlgClass ui;
};

#endif // DETAILTITLEEDITDLG_H

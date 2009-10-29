#ifndef DEPEDITDLG_H
#define DEPEDITDLG_H

#include <QDialog>
#include "ui_depeditdlg.h"

class DepEditDlg : public QDialog
{
	Q_OBJECT

public:
	DepEditDlg(QWidget *parent = 0);
	~DepEditDlg();

public Q_SLOTS:
	virtual void accept();

public:
	void setData(const QString& depId, const QString& depMemo);
	QString depId() const;
	QString depMemo() const;
	void setEditMode();

private:
	Ui::DepEditDlgClass ui;
};

#endif // DEPEDITDLG_H

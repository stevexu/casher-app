#ifndef TITLEEDITDLG_H
#define TITLEEDITDLG_H

#include <QDialog>
#include "ui_titleeditdlg.h"

class TitleEditDlg : public QDialog
{
	Q_OBJECT

public:
	TitleEditDlg(QWidget *parent = 0);
	~TitleEditDlg();

public Q_SLOTS:
	virtual void accept();

public:
	void setEditMode();
	void setTitleCode(const QString& code);
	QString titleCode() const;
	void setTitleProperty(const QString& property);
	QString titleProperty() const;
	void setTitleMemo(const QString& memo);
	QString titleMemo() const;


private:
	Ui::TitleEditDlgClass ui;
};

#endif // TITLEEDITDLG_H

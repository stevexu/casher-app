#ifndef CRCHECKEDITDLG_H
#define CRCHECKEDITDLG_H

#include <QDialog>
#include "ui_crcheckeditdlg.h"

#ifndef QRecords
typedef QList< QMap< QString, QString > > QRecords;
#endif

class CRCheckEditDlg : public QDialog
{
	Q_OBJECT

public:
	CRCheckEditDlg(QWidget *parent = 0);
	~CRCheckEditDlg();

private:
	void needCheckStateChanged();

public:
	void setEditMode();

	QString checkCode() const;
	void setCheckCode(const QString& code);

	void setDate(const QDate& d);
	QDate date() const;

	void setTitle(const QString& checkTitle);
	QString title() const;						// return the title's code

	void setSubTitle(const QString& checkSubTitle);
	QString subTitle() const;					// return the subtitle's code

	void setDepartment(const QString& checkDep);
	QString department() const;					// return the department's code

	void setReqPeople(const QString& name);
	QString reqPeople() const;					// return the people's name

	void setAccount(const QString& checkAccount);
	QString account() const;					// return the account's code

	void setMoney(const QString& checkMoney);
	QString money() const;

	void setMemo(const QString& checkMemo);
	QString memo() const;

private:
	Ui::CRCheckEditDlgClass ui;

	QRecords m_titleRecords;
	QRecords m_subTitleRecords;
	QRecords m_accountRecords;
	QRecords m_depRecords;
};

#endif // CRCHECKEDITDLG_H

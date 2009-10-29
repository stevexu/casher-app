#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QtCore>

#include "qmysql.h"

// CONST DEFINE
// For department table
static const char* DepartmentTableName = "department";
static const char* depIdColumnName = "dep_code";
static const char* depMemoColumnName = "dep_description";
// For bank account table
static const char* BankAccountTableName = "bank";
static const char* accountCodeColumnName = "account_code";
static const char* accountDescriptionColumnName = "account_description";
static const char* bankNameColumnName = "bank_name";
static const char* bankAccountColumnName = "bank_account";
static const char* isCashColumnName = "is_cash_account";
// For budget table
static const char* BudgetTableName = "budget";
static const char* budgetMonthColumnName = "budget_month";
static const char* budgetIdColumnName = "budget_id";
static const char* titleColumnName = "title_code";
static const char* subTitleColumnName = "sec_title_code";
static const char* budgetColumnName = "budget";
static const char* budgetFinalMonthColumnName = "final_month";
static const char* budgetFinalStatusColumnName = "final_status";
static const char* budgetMemoColumnName = "memo";
static const char* budgetCreateDateColumnName = "budget_create_date";
// For Title table
static const char* TitleTableName = "subtitle";
static const char* titleCodeColumnName = "title_code";
static const char* titlePropertyColumnName = "money_direction";
static const char* titleMemoColumnName = "title_description";
// For detail title table
static const char* DetailTitleTableName = "detail_subtitle";
static const char* detailTitleCodeColumnName = "title_code";
static const char* detailTitleMemoColumnName = "title_description";
// For Income table
static const char* IncomeTableName = "income";
static const char* incomeCodeColumnName = "code";
static const char* incomeDateColumnName = "date";
static const char* incomeAccountColumnName = "account";
static const char* incomeTitleColumnName = "title";
static const char* incomeSubTitleColumnName = "subtitle";
static const char* incomeMoneyColumnName = "money";
static const char* incomeMemoColumnName = "memo";

// For CR Check table
static const char* CRCheckTableName = "cr_check";
static const char* checkDepCodeColumnName = "dep_code";
static const char* checkTitleCodeColumnName = "title_code";
static const char* checkSubTitleCodeColumnName = "sub_title_code";
static const char* checkReqMoneyColumnName = "req_money";
static const char* checkReqDateColumnName = "req_date";
static const char* checkStrikeDateColumnName = "strike_date";
static const char* checkMoneyDirectionColumnName = "money_direction";
static const char* checkCodeColumnName = "req_id";
static const char* checkAccountCodeColumnName = "account_code";
static const char* checkReqPeopleColumnName = "req_people";
static const char* checkIsVerifiedColumnName = "is_verified";
static const char* checkMemoColumnName = "memo";

static const QString strYes = QString::fromLocal8Bit("是");
static const QString strNo = QString::fromLocal8Bit("否");
static const QString JIEAN = QString::fromLocal8Bit("已結");
static const QString WEIJIE = QString::fromLocal8Bit("未结案");
static const QString ZHUANXIAYUE = QString::fromLocal8Bit("转下月");
static const QString JIE = QString::fromLocal8Bit("借");
static const QString DAI = QString::fromLocal8Bit("贷");

#ifndef QArray
typedef QMap<QString, QString> QArray;
#endif

typedef QList<QArray> QRecords;

QString getMemo(const QRecords& records, const QString& code, const QString& codeValue, const QString& memoKey);

class DatabaseHandler : public QObject
{
	Q_OBJECT

public:
	static DatabaseHandler* instance()
	{
		static QMutex mutext;
		if (!m_instance)
		{
			mutext.lock();
			if (!m_instance)
			{
				m_instance = new DatabaseHandler;
			}
			mutext.unlock();
		}

		return m_instance;
	}

	~DatabaseHandler();

public:
	Database* getHandler();
	QRecords loadTable(const char* tableName);

private:
	DatabaseHandler(QObject *parent = 0);

private:
	static  DatabaseHandler* m_instance;
	Database* m_db;
	QRecords m_depRecords;			// department records
	QRecords m_bankRecords;			// bank account records
};

#endif // DATABASEHANDLER_H

#include "databasehandler.h"
#include "QtLog.h"

#ifdef _DEBUG
#define Database_Debug(msg){QString str; str+= msg; str+="\r\n"; QtLog::instance()->appendLog(msg);}
#else
#define Database_Debug(msg)
#endif

DatabaseHandler* DatabaseHandler::m_instance = 0;

DatabaseHandler::DatabaseHandler(QObject *parent)
	: QObject(parent)
{
	m_db = new QMySql;
	m_db->connect("127.0.0.1", 3306, "gdf", "root", "");
}

DatabaseHandler::~DatabaseHandler()
{
	if (m_db)
	{
		m_db->close();
		delete m_db;
	}
}


Database* DatabaseHandler::getHandler()
{
	return m_db;
}

QRecords DatabaseHandler::loadTable(const char* tableName)
{
	Database_Debug("DatabaseHandler::loadTable Entry")
	QRecords records;

	Database_Result& dr = getHandler()->from(tableName)->get();
	if (dr.isValid() && dr.count() <= 0)
	{
		QString strError = QString("DatabaseHandler::loadTable --- Table Name:%1, no records or query error").arg(tableName);
		Database_Debug(strError)
		return records;
	}

	ResultArray& rAry = dr.resultArray();
	for (ResultArray::const_iterator itorArray = rAry.begin(); itorArray != rAry.end(); ++itorArray)
	{
		QArray rowData;
		for (Array::const_iterator itor = itorArray->begin(); itor != itorArray->end(); ++itor)
		{
			QString key = QString::fromUtf8(itor->first.c_str());
			QString value = QString::fromUtf8(itor->second.c_str());
			rowData[key] = value;
		}
		records.push_back(rowData);
	}
	Database_Debug("DatabaseHandler::loadTable Exit")

	return records;
}

QString getMemo(const QRecords &records, const QString& code, const QString& codeValue, const QString& memoKey)
{
	for (int i = 0; i < records.count(); i++)
	{
		QArray record = records.at(i);
		if (record[code] == codeValue)
		{
			return record[memoKey];
		}
	}

	return QString();
}
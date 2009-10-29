#include <QtGui>
#include "qmysql.h"

#pragma warning(disable:4005)

QMySql::QMySql():
m_bConnected(false)
{
	m_queryKey.clear();
	m_queryResult = NULL;
}

QMySql::~QMySql()
{
	close();
	if (m_queryResult)
	{
		delete m_queryResult;
	}
}

bool QMySql::connect(const char* host, unsigned short port, const char* databasename, const char* user, const char* password){

	if (!mysql_init(&m_database))
	{
		m_bConnected = false;
		return false;
	}
	
	if (!mysql_real_connect(&m_database, host, user, password, databasename, port, 0, 0))
	{
		QMessageBox::about(NULL, ("qmysql::connect"), ("failed connect"));
		m_bConnected = false;
		return false;
	} 

	m_bConnected = true;

	mysql_set_character_set(&m_database, "utf8");

	return true;
}

std::string QMySql::getKey(std::pair<std::string, std::string>& p){
	return p.first;
}

bool QMySql::insert(const char* table, const Array& values){
	
	StringList keys, keyValues;

	for (Array::const_iterator itor = values.begin(); itor != values.end(); ++itor)
	{
		keys.push_back(itor->first);
		keyValues.push_back(itor->second);
	}

	char buf[MaxSqlCommandSize];
	memset(buf, 0, MaxSqlCommandSize);
	sprintf_s(buf, MaxSqlCommandSize, "INSERT INTO `%s`.`%s` ( %s ) VALUES ( %s );", 
		m_database.db, 
		table, 
		buildKeys(keys).c_str(), 
		buildValues(keyValues).c_str());

	if (0 != mysql_query(&m_database, buf))
	{
		QMessageBox::about(NULL, "query error", mysql_error(&m_database));
		return false;
	}

	return true;
}

void QMySql::close()
{
	if (m_bConnected)
	{
		mysql_close(&m_database);
		m_bConnected = false;
	}
}


std::string QMySql::buildKeys(const StringList& keys){

	std::string outStr;

	foreach(std::string str, keys){
		outStr += "`";
		outStr += str + "`,";
	}

	outStr.erase(outStr.size() - 1, 1);

	return outStr;
}

std::string QMySql::buildValues(const StringList& values){

	std::string outStr;

	foreach(std::string str, values){
		outStr += "'";
		outStr += str + "',";
	}

	outStr.erase(outStr.size() - 1, 1);

	return outStr;
}

Database* QMySql::where(const char* expression)
{
	if (m_queryKey.find("where") == m_queryKey.end())
	{
		// not find the key in the map, add it to the map
		m_queryKey["where"] = expression;
	}

	return this;
}

Database* QMySql::limit(const char* expression)
{
	if (m_queryKey.find("limit") == m_queryKey.end())
	{
		m_queryKey["limit"] = expression;
	}
	
	return this;
}

Database* QMySql::from(const char* table)
{
	m_queryKey["from"] = table;

	return this;
}

Database_Result& QMySql::query(const char *sql)
{
	if (!m_queryResult)
	{
		m_queryResult = new Database_Result;
	}
	m_queryResult->reset();

	if (mysql_real_query(&m_database, sql, strlen(sql)))
	{
		// query error
		m_queryResult->setErrMsg(mysql_error(&m_database));
		m_queryResult->setErrNo(mysql_errno(&m_database));
		return *m_queryResult;
	}

	MYSQL_RES* result = mysql_store_result(&m_database);

	if (result)
	{
		// has result
		StringList fields;
		MYSQL_FIELD* field;
		while ((field = mysql_fetch_field(result)))
		{
			fields.push_back(field->name);
		}

		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result)))
		{
			int i = 0;
			Array record;
			foreach(std::string fieldName, fields)
			{
				record[fieldName] = (row[i]==0)?"":row[i];
				i++;
			}
			m_queryResult->addRecord(record);
		}

		if (!m_queryResult->hasRecord())
		{
			std::string errMsg = sql;
			errMsg += ": No Record";
			m_queryResult->setErrMsg(errMsg.c_str());
		}

		mysql_free_result(result);
	}
	else
	{
		// not record
		if (mysql_field_count(&m_database) == 0)
		{
			// query do not return any data, like INSERT
			size_t num_rows = mysql_affected_rows(&m_database);
			m_queryResult->setAffectRowNum(num_rows);		
		}
		else
		{
			// error
			m_queryResult->setErrNo(mysql_errno(&m_database));
			m_queryResult->setErrMsg(mysql_error(&m_database));
		}
	}

	return *m_queryResult;
}

Database* QMySql::orderBy(const char* expression)
{
	if (m_queryKey.find("orderby") == m_queryKey.end())
	{
		m_queryKey["orderby"] = expression;
	}

	return this;
}

Database_Result& QMySql::get()
{
	char buf[MaxSqlCommandSize];
	memset(buf, 0, MaxSqlCommandSize);

	if (m_queryKey.find("from") == m_queryKey.end())
	{
		return *m_queryResult;
	}

	sprintf_s(buf, MaxSqlCommandSize, "SELECT * FROM `%s`.`%s` ", m_database.db, m_queryKey["from"].c_str());

	std::string strSql = buf;
	if (m_queryKey.find("orderby") != m_queryKey.end())
	{
		memset(buf, 0, MaxSqlCommandSize);
		sprintf_s(buf, MaxSqlCommandSize, "ORDER BY %s ", m_queryKey["orderby"].c_str());
		strSql += buf;
	}

	if (m_queryKey.find("where") != m_queryKey.end())
	{
		memset(buf, 0, MaxSqlCommandSize);
		sprintf_s(buf, MaxSqlCommandSize, "WHERE %s ", m_queryKey["where"].c_str());
		strSql += buf;
	}

	if (m_queryKey.find("limit") != m_queryKey.end())
	{
		memset(buf, 0, MaxSqlCommandSize);
		sprintf_s(buf, MaxSqlCommandSize, "LIMIT %s ", m_queryKey["limit"].c_str());
		strSql += buf;
	}

	m_queryKey.clear();

	return query(strSql.c_str());
}

bool QMySql::update(const char* table, const Array& values, const Array& wheres)
{
	char buf[MaxSqlCommandSize];
	memset(buf, 0, MaxSqlCommandSize);

	sprintf_s(buf, MaxSqlCommandSize, "UPDATE `%s`.`%s` SET ", m_database.db, table);

	std::string strSql = buf;

	for (Array::const_iterator itor = values.begin(); itor != values.end(); ++itor)
	{
		memset(buf, 0 ,MaxSqlCommandSize);
		sprintf_s(buf, MaxSqlCommandSize, "`%s` = '%s', ", itor->first.c_str(), itor->second.c_str());
		strSql += buf;
	}

	strSql.erase(strSql.size() - 2, 2);	// remove the ',' and space 

	strSql += " WHERE ";

	for (Array::const_iterator itor = wheres.begin(); itor != wheres.end(); ++itor)
	{
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, sizeof(buf), "`%s`='%s', ", itor->first.c_str(), itor->second.c_str());
		strSql += buf;
	}

	strSql.erase(strSql.size() - 2, 2);	// remove the ',' symbol and space 

	Database_Result& dr = query(strSql.c_str());

	if (!dr.isValid())
	{
		return false;
	}
	
	return (dr.affectRowNum() > 0);
}

bool QMySql::del(const char *table, const Array &wheres)
{
	char buf[MaxSqlCommandSize];
	memset(buf, 0 ,sizeof(buf));
	sprintf_s(buf, sizeof(buf), "DELETE FROM `%s`.`%s` WHERE ", m_database.db, table);
	std::string strSql = buf;

	for (Array::const_iterator itor = wheres.begin(); itor != wheres.end(); ++itor)
	{
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, sizeof(buf), "`%s` = '%s',", itor->first.c_str(), itor->second.c_str());
		strSql += buf;
	}

	strSql.erase(strSql.size() - 1, 1);

	Database_Result& dr = query(strSql.c_str());

	if (!dr.isValid())
	{
		return false;
	}

	return (dr.affectRowNum() > 0);
}
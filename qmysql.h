#ifndef QMYSQL_H
#define QMYSQL_H

#include "Database.h"
#include <mysql.h>

class QMySql: public Database
{
public:
	QMySql();
	~QMySql();

public:
	// Derived from Database
	virtual bool connect(const char* ip, unsigned short port, const char* databasename, const char* user, const char* password);
	virtual bool insert(const char* table, const Array& values);
	virtual Database* where(const char* expression);
	virtual Database* limit(const char* expression);
	virtual Database* orderBy(const char* expression);
	virtual Database* from(const char* table);
	virtual Database_Result& get();
	virtual Database_Result& query(const char* sql);
	virtual bool update(const char* table, const Array& values, const Array& wheres);
	virtual bool del(const char* table, const Array& wheres);
	virtual void close();

protected:
	// derived from the Database
	virtual std::string buildKeys(const StringList& keys);
	virtual std::string buildValues(const StringList& values);

private:
	static std::string getKey(std::pair<std::string, std::string>& p);

private:
	MYSQL m_database;
	bool m_bConnected;
	Array m_queryKey;
	Database_Result* m_queryResult;
};

#endif // QMYSQL_H

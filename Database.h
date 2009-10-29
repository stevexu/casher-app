#pragma once

#ifndef _DATABASE_DRIVER_H_
#define _DATABASE_DRIVER_H_

#define  MaxSqlCommandSize	4096

#include "Database_Result.h"

class Database
{
public:
	Database(void);
	virtual ~Database(void);

public:
	virtual bool connect(const char* ip, unsigned short port, const char* databasename, const char* user, const char* password);
	virtual bool insert(const char* table, const Array& values);
	virtual Database* where(const char* expression) = 0;
	virtual Database* limit(const char* expression) = 0;
	virtual Database* orderBy(const char* expression) = 0;	
	virtual Database_Result& get() = 0;
	virtual Database* from(const char* table) = 0;
	virtual Database_Result& query(const char* sql) = 0;
	virtual bool update(const char* table, const Array& values, const Array& wheres) = 0;
	virtual bool del(const char* table, const Array& wheres) = 0;
	virtual void close();

protected:
	virtual std::string buildKeys(const StringList& keys) = 0;
	virtual std::string buildValues(const StringList& values) = 0;

private:
	bool m_bConnected;
};

#endif

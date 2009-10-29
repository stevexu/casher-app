#ifndef _DATABASE_RESULT_H_
#define _DATABASE_RESULT_H_

#pragma once

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>

typedef	 std::list<std::string>	StringList;
typedef  std::map<std::string, std::string> Array;
typedef  std::list<Array> ResultArray;

class Database_Result
{
public:
	Database_Result(void);
	virtual ~Database_Result(void);

public:
	size_t count() const;
	StringList getRow(const char* row) const;
	size_t getRowNum() const;
	ResultArray& resultArray() const;

	void addRecord(const Array& record);

	std::string errorMsg() const;
	unsigned int errNo() const;
	unsigned int affectRowNum() const;
	void setErrMsg(const char* err);
	void setErrNo(unsigned int errNo);
	void setAffectRowNum(unsigned int rowNum);

	bool isValid() const
	{return (m_errNo == 0);}
	bool hasRecord() const;

	void reset();

public:
	Database_Result& operator = (const Database_Result& dr);

private:
	ResultArray* m_resultArray;
	std::string m_errorMsg;
	unsigned int m_errNo;
	unsigned int m_affectRowsNum;
};

#endif

#include "Database_Result.h"

Database_Result::Database_Result(void)
{
	m_resultArray = NULL;
	m_errorMsg = "";
	m_errNo = 0;
	m_affectRowsNum = 0;
}

Database_Result::~Database_Result(void)
{
	reset();
}

size_t Database_Result::count() const
{
	if (m_resultArray)
	{
		return m_resultArray->size();
	}

	return 0;
}

size_t Database_Result::getRowNum() const
{
	if (m_resultArray)
	{
		ResultArray::iterator itor = m_resultArray->begin();
		return itor->size();
	}

	return 0;
}

StringList Database_Result::getRow(const char *row) const{
	// test if the row is exist
	if (!m_resultArray)
	{
		return StringList();
	}

	ResultArray::iterator itor = m_resultArray->begin();
	if (itor->find(row) == itor->end())
	{
		// not found the specified row
		return StringList();
	}

	StringList rows;

	for (ResultArray::iterator itor = m_resultArray->begin(); itor != m_resultArray->end(); ++itor)
	{
		rows.push_back((*itor)["row"]);
	}

	return rows;
}

ResultArray& Database_Result::resultArray() const
{
	return *m_resultArray;
}

void Database_Result::reset()
{
	if (m_resultArray)
	{
		for (ResultArray::iterator itor = m_resultArray->begin(); itor != m_resultArray->end(); ++itor)
		{
			itor->clear();
		}

		m_resultArray->clear();
		delete m_resultArray;
		m_resultArray = NULL;
	}

	m_affectRowsNum = 0;
	m_errNo = 0;
	m_errorMsg = "";
}

void Database_Result::setErrNo(unsigned int errNo)
{
	m_errNo = errNo;
}

void Database_Result::setErrMsg(const char *err)
{
	m_errorMsg = err;
}

unsigned int Database_Result::errNo() const
{
	return m_errNo;
}

std::string Database_Result::errorMsg() const{
	return m_errorMsg;
}

void Database_Result::setAffectRowNum(unsigned int rowNum)
{
	m_affectRowsNum = rowNum;
}

unsigned int Database_Result::affectRowNum() const
{
	return m_affectRowsNum;
}

Database_Result& Database_Result::operator = (const Database_Result& dr)
{
	if (this == &dr)
	{
		return *this;
	}

	m_affectRowsNum = dr.m_affectRowsNum;
	m_errNo = dr.m_errNo;
	m_errorMsg = dr.m_errorMsg;
	m_resultArray = dr.m_resultArray;

	return *this;
}

bool Database_Result::hasRecord() const
{
	if (NULL == m_resultArray)
	{
		return false;
	}

	return (m_resultArray->size() > 0);
}

void Database_Result::addRecord(const Array & record)
{
	if (!m_resultArray)
	{
		m_resultArray = new ResultArray;
	}

	if (m_resultArray)
	{
		m_resultArray->push_back(record);
	}
}
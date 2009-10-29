#include "Database.h"

Database::Database(void):
m_bConnected(false)
{
}

Database::~Database(void)
{
}


bool Database::connect(const char* ip, unsigned short port, const char* databasename, const char* user, const char* password){
	return false;
}

bool Database::insert(const char* table, const Array& values){
	return false;
}

void Database::close()
{

}
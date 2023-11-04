#pragma once

#include "public/mysql_mm.h"

class CMySQLClient : public IMySQLClient
{
public:
	IMySQLConnection* CreateMySQLConnection(MySQLConnectionInfo info);
};
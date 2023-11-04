#include "mysql_client.h"
#include "database.h"
#include "tier0/dbg.h"

extern std::vector<MySQLConnection*> g_vecMysqlConnections;

IMySQLConnection* CMySQLClient::CreateMySQLConnection(MySQLConnectionInfo info)
{
	auto connection = new MySQLConnection(info);
	g_vecMysqlConnections.push_back(connection);

	return connection;
}
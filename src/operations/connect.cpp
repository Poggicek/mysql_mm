/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
 * Original code from SourceMod
 * Copyright (C) 2004-2014 AlliedModders LLC
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "connect.h"
#include "tier0/dbg.h"

void TConnectOp::RunThreadPart()
{
	m_szError[0] = '\0';
	MYSQL *mysql = mysql_init(NULL);

	if (!mysql)
		Error("Uh oh, mysql is null!");

	const char *host = NULL, *socket = NULL;

	int timeout = 60;

	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout);
	mysql_options(mysql, MYSQL_OPT_READ_TIMEOUT, (const char *)&timeout);
	mysql_options(mysql, MYSQL_OPT_WRITE_TIMEOUT, (const char *)&timeout);

	bool my_true = true;
	mysql_options(mysql, MYSQL_OPT_RECONNECT, (const char *)&my_true); // deprecated

	if (m_pCon->m_info.host[0] == '/')
	{
		host = "localhost";
		socket = host;
	}
	else
	{
		host = m_pCon->m_info.host;
		socket = NULL;
	}

	if (!mysql_real_connect(mysql,
							host,
							m_pCon->m_info.user,
							m_pCon->m_info.pass,
							m_pCon->m_info.database,
							m_pCon->m_info.port,
							socket,
							((1) << 17)))
	{

		mysql_close(mysql);
		strncpy(m_szError, mysql_error(mysql), sizeof m_szError);
		return;
	}

	m_pDatabase = mysql;
}

void TConnectOp::RunThinkPart()
{
	if (m_szError[0])
		ConMsg("Failed to establish a MySQL connection: %s\n", m_szError);

	m_pCon->SetDatabase(m_pDatabase);
	m_callback(m_pDatabase != nullptr);
}

void TConnectOp::CancelThinkPart()
{
	mysql_close(m_pDatabase);
	m_pCon->SetDatabase(nullptr);
}
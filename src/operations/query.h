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

#pragma once
#include "../database.h"
#include "../mysql_result.h"

class TQueryOp : public ThreadOperation
{
public:
	TQueryOp(MySQLConnection* con, std::string query, QueryCallbackFunc func) : m_pCon(con), m_szQuery(query), m_callback(func)
	{

	}

	~TQueryOp();

	void RunThreadPart();
	void CancelThinkPart();
	void RunThinkPart();
private:
	MySQLConnection* m_pCon;
	std::string m_szQuery;
	QueryCallbackFunc m_callback;
	MYSQL_RES* m_res = nullptr;
	CMySQLQuery* m_pQuery = nullptr;
	char m_szError[255];
};
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

#include "query.h"
#include "tier0/dbg.h"
#include "../mysql_result.h"

TQueryOp::~TQueryOp()
{
	delete m_pQuery;
}

void TQueryOp::RunThreadPart()
{
	auto pDatabase = m_pCon->GetDatabase();
	if (mysql_query(pDatabase, m_szQuery))
	{
		ConMsg("MySQL query error: %s", mysql_error(pDatabase));
		return;
	}

	if (mysql_field_count(pDatabase))
		m_res = mysql_store_result(pDatabase);
}

void TQueryOp::RunThinkPart()
{
	m_pQuery = new CMySQLQuery(m_pCon, m_res);

	m_callback(m_pQuery);
}

void TQueryOp::CancelThinkPart()
{
	mysql_close(m_pCon->GetDatabase());
	m_pCon->SetDatabase(nullptr);
}
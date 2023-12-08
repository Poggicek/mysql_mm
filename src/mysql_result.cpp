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

#include <string.h>
#include "mysql_result.h"

CMySQLResult::CMySQLResult(MYSQL_RES* res) : m_pRes(res)
{
	Update();
};

void CMySQLResult::Update()
{
	if(!m_pRes) 
	{
		m_ColCount = 0;
		m_RowCount = 0;
	} 
	else 
	{
		m_ColCount = mysql_num_fields(m_pRes);
		m_RowCount = mysql_num_rows(m_pRes);
	}
}

int CMySQLResult::GetRowCount()
{
	return m_RowCount;
}

int CMySQLResult::GetFieldCount()
{
	return m_ColCount;
}

bool CMySQLResult::FieldNameToNum(const char* name, unsigned int* columnId)
{
	unsigned int total = GetFieldCount();

	for (unsigned int i = 0; i < total; i++)
	{
		if (strcmp(FieldNumToName(i), name) == 0)
		{
			*columnId = i;
			return true;
		}
	}

	return false;
}

const char* CMySQLResult::FieldNumToName(unsigned int colId)
{
	if (colId >= GetFieldCount())
	{
		return NULL;
	}

	MYSQL_FIELD* field = mysql_fetch_field_direct(m_pRes, colId);
	return field ? (field->name ? field->name : "") : "";
}

bool CMySQLResult::MoreRows()
{
	return m_CurRow < m_RowCount;
}

IMySQLRow* CMySQLResult::FetchRow()
{
	if (m_CurRow >= m_RowCount)
	{
		/* Put us one after so we know to block CurrentRow() */
		m_CurRow = m_RowCount + 1;
		return NULL;
	}
	m_Row = mysql_fetch_row(m_pRes);
	m_Lengths = mysql_fetch_lengths(m_pRes);
	m_CurRow++;
	return this;
}

IMySQLRow* CMySQLResult::CurrentRow()
{
	if (!m_pRes
		|| !m_CurRow
		|| m_CurRow > m_RowCount)
	{
		return NULL;
	}

	return this;
}

bool CMySQLResult::Rewind()
{
	mysql_data_seek(m_pRes, 0);
	m_CurRow = 0;
	return true;
}

EMySQLType CMySQLResult::GetFieldType(unsigned int field)
{
	if (field >= m_ColCount)
	{
		return MM_MYSQL_TYPE_UNKNOWN;
	}

	MYSQL_FIELD* fld = mysql_fetch_field_direct(m_pRes, field);
	if (!fld)
	{
		return MM_MYSQL_TYPE_UNKNOWN;
	}

	return (EMySQLType)fld->type;
}

bool CMySQLResult::IsNull(unsigned int columnId)
{
	if (columnId >= m_ColCount)
	{
		return true;
	}

	return (m_Row[columnId] == NULL);
}

char* CMySQLResult::GetString(unsigned int columnId, size_t* length)
{
	if (columnId >= m_ColCount)
	{
		return nullptr;
	}
	else if (m_Row[columnId] == NULL) {
		if (length)
		{
			*length = 0;
		}
		return nullptr;
	}

	if (length)
		*length = (size_t)m_Lengths[columnId];

	return m_Row[columnId];
}

size_t CMySQLResult::GetDataSize(unsigned int columnId)
{
	if (columnId >= m_ColCount)
	{
		return 0;
	}

	return (size_t)m_Lengths[columnId];
}

float CMySQLResult::GetFloat(unsigned int columnId)
{
	if (columnId >= m_ColCount)
	{
		return 0.0f;
	}
	else if (m_Row[columnId] == NULL) {
		return 0.0f;
	}

	return (float)atof(m_Row[columnId]);
}

int CMySQLResult::GetInt(unsigned int columnId)
{
	if (columnId >= m_ColCount)
	{
		return 0;
	}
	else if (m_Row[columnId] == NULL) {
		return 0;
	}

	return atoi(m_Row[columnId]);
}

CMySQLQuery::CMySQLQuery(MySQLConnection* db, MYSQL_RES* res) : m_pDatabase(db), m_res(res)
{
	m_insertId = m_pDatabase->GetInsertID();
	m_affectedRows = m_pDatabase->GetAffectedRows();
}

IMySQLResult* CMySQLQuery::GetResultSet()
{
	if (m_res.m_pRes == NULL)
	{
		return NULL;
	}

	return &m_res;
}

bool CMySQLQuery::FetchMoreResults()
{
	auto pDatabase = m_pDatabase->GetDatabase();
	if (m_res.m_pRes == NULL)
	{
		return false;
	}
	else if (!mysql_more_results(pDatabase)) {
		return false;
	}

	mysql_free_result(m_res.m_pRes);
	m_res.m_pRes = NULL;

	if (mysql_next_result(pDatabase) != 0)
	{
		return false;
	}

	m_res.m_pRes = mysql_store_result(pDatabase);
	m_res.Update();

	return (m_res.m_pRes != NULL);
}

CMySQLQuery::~CMySQLQuery()
{
	while (FetchMoreResults())
	{
		/* Spin until all are gone */
	}

	/* Free the last, if any */
	if (m_res.m_pRes != NULL)
	{
		mysql_free_result(m_res.m_pRes);
	}
}


unsigned int CMySQLQuery::GetInsertId()
{
	return m_insertId;
}

unsigned int CMySQLQuery::GetAffectedRows()
{
	return m_affectedRows;
}

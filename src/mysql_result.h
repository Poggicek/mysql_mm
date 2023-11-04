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

#if WIN32
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include "public/mysql_mm.h"

class CMySQLQuery;

class CMySQLResult : public IMySQLResult, IMySQLRow
{
	friend class CMySQLQuery;
public:
	CMySQLResult(MYSQL_RES* res);

	void Update();

	int GetRowCount();
	int GetFieldCount();
	bool FieldNameToNum(const char* name, unsigned int* columnId);
	const char* FieldNumToName(unsigned int colId);
	bool MoreRows();
	IMySQLRow* FetchRow();
	IMySQLRow* CurrentRow();
	bool Rewind();
	EMySQLType GetFieldType(unsigned int field);
	char* GetString(unsigned int columnId, size_t* length = nullptr);
	size_t GetDataSize(unsigned int columnId);
	float GetFloat(unsigned int columnId);
	int GetInt(unsigned int columnId);
	bool IsNull(unsigned int columnId);

private:
	//MYSQL* m_pDatabase;
	MYSQL_RES* m_pRes;

	unsigned int m_ColCount = 0;
	unsigned int m_RowCount = 0;
	unsigned int m_CurRow = 0;
	MYSQL_ROW m_Row;
	unsigned long* m_Lengths = 0;
};

class CMySQLQuery : public IMySQLQuery
{
	friend class CMySQLResult;
public:
	CMySQLQuery(MYSQL* db, MYSQL_RES* res);
	~CMySQLQuery();
	IMySQLResult* GetResultSet();
	bool FetchMoreResults();
private:
	MYSQL* m_pDatabase;
	CMySQLResult m_res;
};
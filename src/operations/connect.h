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

class TConnectOp : public ThreadOperation
{
public:
	TConnectOp(MySQLConnection* con, ConnectCallbackFunc func) : m_pCon(con), m_callback(func)
	{

	}

	void RunThreadPart();
	void CancelThinkPart()
	{

	}

	void RunThinkPart()
	{

	}
private:
	MySQLConnection* m_pCon;
	ConnectCallbackFunc m_callback;
};
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

#include "database.h"
#include "tier0/dbg.h"

#include "operations/connect.h"
#include "operations/query.h"

extern std::vector<MySQLConnection*> g_vecMysqlConnections;

MySQLConnection::MySQLConnection(const MySQLConnectionInfo info)
{
    this->m_info = info;
}

MySQLConnection::~MySQLConnection()
{
    ConMsg("Destroying MySQL connection %s\n", m_info.database);
	if (m_thread)
	{
        {
            std::lock_guard<std::mutex> lock(m_Lock);
            m_Terminate = true;
            m_QueueEvent.notify_all();
        }

		m_thread->join();
		m_thread.reset();
        m_Terminate = false;
	}

    while (!m_ThinkQueue.empty())
    {
        ThreadOperation* op = m_ThinkQueue.front();
        m_ThinkQueue.pop();

        op->CancelThinkPart();
        delete op;
    }

    if (m_pDatabase)
        mysql_close(m_pDatabase);
}

void MySQLConnection::Connect(ConnectCallbackFunc callback)
{
    TConnectOp* op = new TConnectOp(this, callback);

    AddToThreadQueue(op);
}

void MySQLConnection::Query(char* query, QueryCallbackFunc callback)
{
    if (!m_pDatabase)
    {
        Warning("Failed querying a disconnected database (%s).\n", m_info.host);
        return;
    }

    TQueryOp* op = new TQueryOp(this, query, callback);

    AddToThreadQueue(op);
}

void MySQLConnection::Query(const char* query, QueryCallbackFunc callback)
{
    Query(const_cast<char*>(query), callback);
}

void MySQLConnection::Destroy()
{
    g_vecMysqlConnections.erase(std::remove(g_vecMysqlConnections.begin(), g_vecMysqlConnections.end(), this), g_vecMysqlConnections.end());
    delete this;
}

void MySQLConnection::RunFrame()
{
    if (!m_ThinkQueue.size())
        return;

    ThreadOperation* op;
    {
        std::lock_guard<std::mutex> lock(m_ThinkLock);
        op = m_ThinkQueue.front();
        m_ThinkQueue.pop();
    }

    op->RunThinkPart();
    delete op;
}

void MySQLConnection::ThreadRun()
{
    if (mysql_thread_safe())
        mysql_thread_init();

    std::unique_lock<std::mutex> lock(m_Lock);

    while(true)
    {
        if (m_threadQueue.empty())
        {
            if (m_Terminate)
                return;

            m_QueueEvent.wait(lock);
            continue;
        }

        ThreadOperation* op = m_threadQueue.front();
        m_threadQueue.pop();

        lock.unlock();
        op->RunThreadPart();

        {
            std::lock_guard<std::mutex> think_lock(m_ThinkLock);
            m_ThinkQueue.push(op);
        }

        lock.lock();
    }

    mysql_thread_end();
}

void MySQLConnection::AddToThreadQueue(ThreadOperation* threadOperation)
{
    if(!m_thread)
	    m_thread = std::unique_ptr<std::thread>(new std::thread(&MySQLConnection::ThreadRun, this));

    {
        std::lock_guard<std::mutex> lock(m_Lock);
        m_threadQueue.push(threadOperation);
        m_QueueEvent.notify_one();
    }
}

unsigned int MySQLConnection::GetInsertID()
{
    return mysql_insert_id(m_pDatabase);
}

unsigned int MySQLConnection::GetAffectedRows()
{
    return mysql_affected_rows(m_pDatabase);
}
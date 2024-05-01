/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
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

#ifdef WIN32
#include <WinSock2.h>
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <queue>
#include <thread>
#include <condition_variable>
#include <functional>
#include "public/mysql_mm.h"

class ThreadOperation
{
public:
    virtual void RunThreadPart() = 0;
    virtual void CancelThinkPart() = 0;
    virtual void RunThinkPart() = 0;

private:
};

class MySQLConnection : public IMySQLConnection
{
public:
    MySQLConnection(const MySQLConnectionInfo info);
    ~MySQLConnection();
    void Connect(ConnectCallbackFunc callback);
    void Query(char *query, QueryCallbackFunc callback);
    void Query(const char *query, QueryCallbackFunc callback, ...);
    void Destroy();
    void RunFrame();
    void SetDatabase(MYSQL *db) { m_pDatabase = db; }
    MYSQL *GetDatabase() { return m_pDatabase; }
    unsigned int GetInsertID();
    unsigned int GetAffectedRows();
    std::string Escape(char *string);
    std::string Escape(const char *string);

    MySQLConnectionInfo m_info;

private:
    void ThreadRun();
    void AddToThreadQueue(ThreadOperation *threadOperation);

    std::queue<ThreadOperation *> m_threadQueue;
    std::queue<ThreadOperation *> m_ThinkQueue;
    std::unique_ptr<std::thread> m_thread;
    std::condition_variable m_QueueEvent;
    std::mutex m_Lock;
    std::mutex m_ThinkLock;
    bool m_Terminate = false;
    MYSQL *m_pDatabase = nullptr;
};
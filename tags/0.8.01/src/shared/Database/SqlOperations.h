/* 
 * Copyright (C) 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __SQLOPERATIONS_H
#define __SQLOPERATIONS_H

#include "Common.h"

#include "zthread/LockedQueue.h"
#include "zthread/FastMutex.h"
#include "zthread/Thread.h"
#include <queue>
#include "Utilities/Callback.h"

/// ---- BASE ---

class Database;
class SqlDelayThread;

class SqlOperation
{
    public:
        virtual void OnRemove() { delete this; }
        virtual void Execute(Database *db) = 0;
        virtual ~SqlOperation() {}
};

/// ---- ASYNC STATEMENTS / TRANSACTIONS ----

class SqlStatement : public SqlOperation
{
    private:
        const char *m_sql;
    public:
        SqlStatement(const char *sql) : m_sql(strdup(sql)){}
        ~SqlStatement() { free((void*)const_cast<char*>(m_sql)); }
        void Execute(Database *db);
};

class SqlTransaction : public SqlOperation
{
    private:
        std::queue<const char *> m_queue;
    public:
        SqlTransaction() {}
        void DelayExecute(const char *sql) { m_queue.push(strdup(sql)); }
        void Execute(Database *db);
};

/// ---- ASYNC QUERIES ----

class SqlQuery;                                             /// contains a single async query
class QueryResult;                                          /// the result of one
class SqlResultQueue;                                       /// queue for thread sync
class SqlQueryHolder;                                       /// groups several async quries
class SqlQueryHolderEx;                                     /// points to a holder, added to the delay thread

class SqlResultQueue : public ZThread::LockedQueue<MaNGOS::IQueryCallback*, ZThread::FastMutex>
{
    public:
        SqlResultQueue() {}
        void Update();
};

class SqlQuery : public SqlOperation
{
    private:
        const char *m_sql;
        MaNGOS::IQueryCallback * m_callback;
        SqlResultQueue * m_queue;
    public:
        SqlQuery(const char *sql, MaNGOS::IQueryCallback * callback, SqlResultQueue * queue)
            : m_sql(strdup(sql)), m_callback(callback), m_queue(queue) {}
        ~SqlQuery() { free((void*)const_cast<char*>(m_sql)); }
        void Execute(Database *db);
};

class SqlQueryHolder
{
    friend class SqlQueryHolderEx;
    private:
        typedef std::pair<const char*, QueryResult*> SqlResultPair;
        std::vector<SqlResultPair> m_queries;
    public:
        SqlQueryHolder() {}
        ~SqlQueryHolder();
        bool Query(const char *sql);
        bool PQuery(const char *format, ...) ATTR_PRINTF(2,3);
        void DummyQuery();
        void Reserve(size_t size);
        QueryResult* GetResult(size_t index);
        void SetResult(size_t index, QueryResult *result);
        void Execute(MaNGOS::IQueryCallback * callback, SqlDelayThread *thread, SqlResultQueue *queue);
};

class SqlQueryHolderEx : public SqlOperation
{
    private:
        SqlQueryHolder * m_holder;
        MaNGOS::IQueryCallback * m_callback;
        SqlResultQueue * m_queue;
    public:
        SqlQueryHolderEx(SqlQueryHolder *holder, MaNGOS::IQueryCallback * callback, SqlResultQueue * queue)
            : m_holder(holder), m_callback(callback), m_queue(queue) {}
        void Execute(Database *db);
};
#endif                                                      //__SQLOPERATIONS_H

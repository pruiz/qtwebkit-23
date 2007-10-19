/*
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "config.h"
#include "DatabaseTask.h"

#include "Database.h"
#include "Logging.h"
#include "SQLValue.h"
#include "SQLCallback.h"

namespace WebCore {

DatabaseTask::DatabaseTask()
    : m_complete(false)
    , m_synchronous(false)
{
}

DatabaseTask::~DatabaseTask()
{
}

void DatabaseTask::performTask(Database* db)
{
    // Database tasks are meant to be used only once, so make sure this one hasn't been performed before
    ASSERT(!m_complete);

    LOG(StorageAPI, "Performing DatabaseTask %p\n", this);

    if (m_synchronous)
        m_synchronousMutex.lock();

    db->resetAuthorizer();
    doPerformTask(db);
    db->performPolicyChecks();

    m_complete = true;

    if (m_synchronous) {
        m_synchronousCondition.signal();
        m_synchronousMutex.unlock();
    }
}

void DatabaseTask::lockForSynchronousScheduling()
{
    m_synchronousMutex.lock();
    m_synchronous = true;
}

void DatabaseTask::waitForSynchronousCompletion()
{
    // Caller of this method must lock this object beforehand
    ASSERT(m_synchronousMutex.tryLock() == false);

    m_synchronousCondition.wait(m_synchronousMutex);
    m_synchronous = false;
    m_synchronousMutex.unlock();
}

// *** DatabaseOpenTask ***
// Opens the database file and verifies the version matches the expected version

DatabaseOpenTask::DatabaseOpenTask()
    : DatabaseTask()
    , m_code(0)
    , m_success(false)
{
}

void DatabaseOpenTask::doPerformTask(Database* db)
{
    m_success = db->performOpenAndVerify(m_code);
}

// *** DatabaseExecuteSqlTask ***
// Runs the passed in sql query along with the arguments, and calls the callback with the results

DatabaseExecuteSqlTask::DatabaseExecuteSqlTask(const String& query, const Vector<SQLValue>& arguments, PassRefPtr<SQLCallback> callback)
    : DatabaseTask()
    , m_query(query.copy())
    , m_arguments(arguments)
    , m_callback(callback)
{
    ASSERT(callback->isThreadSafe());
}

void DatabaseExecuteSqlTask::doPerformTask(Database* db)
{
    db->performExecuteSql(m_query, m_arguments, m_callback);
}

// *** DatabaseChangeVersionTask ***
// Atomically verifies the current version is the same as the passed in "old version", and changes it to the new version

DatabaseChangeVersionTask::DatabaseChangeVersionTask(const String& oldVersion, const String& newVersion, PassRefPtr<VersionChangeCallback> callback)
    : m_oldVersion(oldVersion.copy())
    , m_newVersion(newVersion.copy())
    , m_callback(callback)
{
    ASSERT(callback->isThreadSafe());
}

void DatabaseChangeVersionTask::doPerformTask(Database* db)
{
    db->performChangeVersion(m_oldVersion, m_newVersion, m_callback);
}

// *** DatabaseTableNamesTask ***
// Retrieves a list of all tables in the database - for WebInspector support

DatabaseTableNamesTask::DatabaseTableNamesTask()
{
}

void DatabaseTableNamesTask::doPerformTask(Database* db)
{
    m_tableNames = db->performGetTableNames();
}


} // namespace WebCore

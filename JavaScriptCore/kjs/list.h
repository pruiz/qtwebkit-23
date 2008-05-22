/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2007 Apple Computer, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef KJS_LIST_H
#define KJS_LIST_H

#include <kjs/value.h>
#include <wtf/HashSet.h>
#include <wtf/Noncopyable.h>
#include <wtf/Vector.h>

namespace KJS {

    class JSValue;
    class List;
    
    class List : Noncopyable {
    private:
        typedef Vector<JSValue*, 8> VectorType;
        typedef HashSet<List*> ListSet;

    public:
        typedef VectorType::iterator iterator;
        typedef VectorType::const_iterator const_iterator;

        // Constructor for a read-write list, to which you may append values.
        // FIXME: Remove all clients of this API, then remove this API.
        List()
            : m_isInMarkSet(false)
#ifndef NDEBUG
            , m_isReadOnly(false)
#endif
        {
            m_buffer = m_vector.data();
            m_size = m_vector.size();
        }

        // Constructor for a read-only list whose data has already been allocated elsewhere.
        List(JSValue** buffer, size_t size)
            : m_buffer(buffer)
            , m_size(size)
            , m_isInMarkSet(false)
#ifndef NDEBUG
            , m_isReadOnly(true)
#endif
        {
        }

        ~List()
        {
            if (m_isInMarkSet)
                markSet().remove(this);
        }

        size_t size() const { return m_size; }
        bool isEmpty() const { return !m_size; }

        JSValue* at(size_t i) const
        {
            if (i < m_size)
                return m_buffer[i];
            return jsUndefined();
        }

        JSValue* operator[](int i) const { return at(i); }

        void clear()
        {
            m_vector.clear();
            m_size = 0;
        }

        void append(JSValue* v)
        {
            ASSERT(!m_isReadOnly);
            
            if (m_vector.size() < m_vector.capacity()) {
                m_vector.uncheckedAppend(v);
                ++m_size;
            } else {
                // Putting the slow "expand and append" case all in one 
                // function measurably improves the performance of the fast 
                // "just append" case.
                expandAndAppend(v);
                m_buffer = m_vector.data();
                ++m_size;
            }
        }

        void getSlice(int startIndex, List& result) const;

        iterator begin() { return m_buffer; }
        iterator end() { return m_buffer + m_size; }

        const_iterator begin() const { return m_buffer; }
        const_iterator end() const { return m_buffer + m_size; }

        static void markProtectedLists()
        {
            if (!markSet().size())
                return;
            markProtectedListsSlowCase();
        }

    private:
        static ListSet& markSet();
        static void markProtectedListsSlowCase();

        void expandAndAppend(JSValue*);
        
        JSValue** m_buffer;
        size_t m_size;

        VectorType m_vector;
        bool m_isInMarkSet;
#ifndef NDEBUG
        bool m_isReadOnly;
#endif

    private:
        // Prohibits new / delete, which would break GC.
        void* operator new(size_t);
        void operator delete(void*);

        void* operator new[](size_t);
        void operator delete[](void*);

        void* operator new(size_t, void*);
        void operator delete(void*, size_t);
    };
    
} // namespace KJS

#endif // KJS_LIST_H

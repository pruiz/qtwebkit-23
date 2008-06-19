/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2007, 2008 Apple Inc. All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef RegExpObject_h
#define RegExpObject_h

#include "FunctionPrototype.h"
#include "regexp.h"

namespace KJS {

    struct RegExpConstructorPrivate;

    class RegExpPrototype : public JSObject {
    public:
        RegExpPrototype(ExecState*, ObjectPrototype*, FunctionPrototype*);

        virtual const ClassInfo* classInfo() const { return &info; }
        static const ClassInfo info;
    };

    class RegExpObject : public JSObject {
    public:
        enum { Global, IgnoreCase, Multiline, Source, LastIndex };

        RegExpObject(RegExpPrototype*, PassRefPtr<RegExp>);
        virtual ~RegExpObject();

        void setRegExp(PassRefPtr<RegExp> r) { m_regExp = r; }
        RegExp* regExp() const { return m_regExp.get(); }

        JSValue* test(ExecState*, const ArgList& args);
        JSValue* exec(ExecState*, const ArgList& args);

        virtual CallType getCallData(CallData&);
        virtual JSValue* callAsFunction(ExecState*, JSObject*, const ArgList&);

        bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
        JSValue* getValueProperty(ExecState*, int token) const;
        void put(ExecState*, const Identifier&, JSValue*);
        void putValueProperty(ExecState*, int token, JSValue*);

        virtual const ClassInfo* classInfo() const { return &info; }
        static const ClassInfo info;

        void setLastIndex(double lastIndex) { m_lastIndex = lastIndex; }

    private:
        bool match(ExecState*, const ArgList& args);

        RefPtr<RegExp> m_regExp;
        double m_lastIndex;
    };

    class RegExpConstructor : public InternalFunction {
    public:
        enum { Dollar1, Dollar2, Dollar3, Dollar4, Dollar5, Dollar6, Dollar7, Dollar8, Dollar9, 
               Input, Multiline, LastMatch, LastParen, LeftContext, RightContext };

        RegExpConstructor(ExecState*, FunctionPrototype*, RegExpPrototype*);

        virtual ConstructType getConstructData(ConstructData&);
        virtual JSObject* construct(ExecState*, const ArgList&);

        virtual JSValue* callAsFunction(ExecState*, JSObject*, const ArgList&);

        virtual void put(ExecState*, const Identifier&, JSValue*);
        void putValueProperty(ExecState*, int token, JSValue*);
        virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
        JSValue* getValueProperty(ExecState*, int token) const;

        virtual const ClassInfo* classInfo() const { return &info; }
        static const ClassInfo info;

        void performMatch(RegExp*, const UString&, int startOffset, int& position, int& length, int** ovector = 0);
        JSObject* arrayOfMatches(ExecState*) const;
        const UString& input() const;

    private:
        JSValue* getBackref(ExecState*, unsigned) const;
        JSValue* getLastParen(ExecState*) const;
        JSValue* getLeftContext(ExecState*) const;
        JSValue* getRightContext(ExecState*) const;

        OwnPtr<RegExpConstructorPrivate> d;
    };

} // namespace

#endif

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef HTMLScriptElement_h
#define HTMLScriptElement_h

#include "CachedResourceClient.h"
#include "HTMLElement.h"

namespace WebCore {

class CachedScript;

class HTMLScriptElement : public HTMLElement, public CachedResourceClient {
public:
    HTMLScriptElement(Document*);
    ~HTMLScriptElement();

    virtual HTMLTagStatus endTagRequirement() const { return TagStatusRequired; }
    virtual int tagPriority() const { return 1; }
    virtual bool checkDTD(const Node* newChild) { return newChild->isTextNode(); }

    virtual void parseMappedAttribute(MappedAttribute*);
    virtual void insertedIntoDocument();
    virtual void removedFromDocument();
    virtual void notifyFinished(CachedResource*);

    virtual void childrenChanged(bool changedByParser = false);

    virtual bool isURLAttribute(Attribute*) const;

    void setCreatedByParser(bool createdByParser) { m_createdByParser = createdByParser; }
    virtual void finishParsingChildren();

    bool shouldExecuteAsJavaScript();
    void evaluateScript(const String& url, const String& script);

    String text() const;
    void setText(const String&);

    String htmlFor() const;
    void setHtmlFor(const String&);

    String event() const;
    void setEvent(const String&);

    String charset() const;
    void setCharset(const String&);

    bool defer() const;
    void setDefer(bool);

    String src() const;
    void setSrc(const String&);

    String type() const;
    void setType(const String&);

private:
    CachedScript* m_cachedScript;
    bool m_createdByParser;
    bool m_evaluated;
};

} //namespace

#endif

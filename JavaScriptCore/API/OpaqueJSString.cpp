/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "OpaqueJSString.h"

#include <kjs/ExecState.h>
#include <kjs/identifier.h>

using namespace KJS;

PassRefPtr<OpaqueJSString> OpaqueJSString::create(const UString& ustring)
{
    if (!ustring.isNull())
        return adoptRef(new OpaqueJSString(ustring.data(), ustring.size()));
    return 0;
}

UString OpaqueJSString::ustring() const
{
    if (this && m_characters)
        return UString(m_characters, m_length, true);
    return UString::null();
}

Identifier OpaqueJSString::identifier(ExecState* exec) const
{
    return identifier(&exec->globalData());
}

Identifier OpaqueJSString::identifier(JSGlobalData* globalData) const
{
    if (!this || !m_characters)
        return Identifier(globalData, static_cast<const char*>(0));

    return Identifier(globalData, m_characters, m_length);
}

/*
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * Copyright (C) 2006 George Staikos <staikos@kde.org>
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2006 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2008 Collabora, Ltd.
 *
 * All rights reserved.
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

#include "AXObjectCache.h"
#include "CString.h"
#include "CachedResource.h"
#include "CookieJar.h"
#include "Cursor.h"
#include "Font.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FTPDirectoryDocument.h"
#include "IntPoint.h"
#include "Widget.h"
#include "GraphicsContext.h"
#include "Cursor.h"
#include "loader.h"
#include "FileSystem.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "GlobalHistory.h"
#include "IconLoader.h"
#include "IntPoint.h"
#include "KURL.h"
#include "Language.h"
#include "loader.h"
#include "LocalizedStrings.h"
#include "Node.h"
#include "NotImplemented.h"
#include "Path.h"
#include "PlatformMouseEvent.h"
#include "PluginDatabase.h"
#include "PluginPackage.h"
#include "RenderTheme.h"
#include "SharedBuffer.h"
#include "SystemTime.h"
#include "TextBoundaries.h"
#include "Widget.h"
#include <stdio.h>
#include <stdlib.h>

using namespace WebCore;

PluginSet PluginDatabase::getPluginsInPaths() const { notImplemented(); return PluginSet(); }
Vector<String> PluginDatabase::defaultPluginPaths() { notImplemented(); return Vector<String>(); }
bool PluginDatabase::isPreferredPluginPath(const String&) const { notImplemented(); return false; }
bool PluginPackage::fetchInfo() { notImplemented(); return false; }
unsigned PluginPackage::hash() const { notImplemented(); return 0; }
bool PluginPackage::equal(const PluginPackage&, const PluginPackage&) { notImplemented(); return false; }
bool PluginPackage::load() { notImplemented(); return false; }

namespace WebCore {

Vector<String> supportedKeySizes() { notImplemented(); return Vector<String>(); }
String signedPublicKeyAndChallengeString(unsigned keySizeIndex, const String &challengeString, const KURL &url) { return String(); }

CString openTemporaryFile(const char* prefix, PlatformFileHandle& handle) { notImplemented(); handle = invalidPlatformFileHandle; return 0; }
void closeFile(PlatformFileHandle&) { notImplemented(); };
int writeToFile(PlatformFileHandle, const char* data, int length) { return -1; };

#if !defined(Q_OS_WIN)
// defined in win/SystemTimeWin.cpp, which is compiled for the Qt/Windows port
float userIdleTime() { notImplemented(); return 0.0; }
#endif

PassRefPtr<SharedBuffer> SharedBuffer::createWithContentsOfFile(const String&) { notImplemented(); return 0; }

}

// vim: ts=4 sw=4 et

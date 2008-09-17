/*
 * Copyright (C) 2004, 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef ScrollbarGtk_h
#define ScrollbarGtk_h

#include "Scrollbar.h"
#include <wtf/PassRefPtr.h>

typedef struct _GtkAdjustment GtkAdjustment;

namespace WebCore {

class ScrollbarGtk : public Scrollbar {
public:
    virtual ~ScrollbarGtk();

    virtual void setFrameGeometry(const IntRect&);
    
    virtual bool handleMouseMoveEvent(const PlatformMouseEvent&) { return false; }
    virtual bool handleMouseOutEvent(const PlatformMouseEvent&) { return false; }
    virtual bool handleMousePressEvent(const PlatformMouseEvent&) { return false; }
    virtual bool handleMouseReleaseEvent(const PlatformMouseEvent&) { return false; }

protected:
    ScrollbarGtk(ScrollbarClient*, ScrollbarOrientation, ScrollbarControlSize);

    virtual void updateThumbPosition();
    virtual void updateThumbProportion();
    virtual void geometryChanged();
    
private:
    static void gtkValueChanged(GtkAdjustment*, ScrollbarGtk*);
    GtkAdjustment* m_adjustment;
};

}

#endif // ScrollbarGtk_h

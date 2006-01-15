/*
 * This file is part of the WebKit project.
 *
 * Copyright (C) 2006 Apple Computer, Inc.
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
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef RenderForeignObject_H
#define RenderForeignObject_H

#include "khtml/rendering/render_block.h"

namespace KSVG {
    class SVGForeignObjectElementImpl;
}

class RenderForeignObject : public khtml::RenderBlock
{
public:
    RenderForeignObject(KSVG::SVGForeignObjectElementImpl *node);
    
    const char *renderName() const { return "RenderForeignObject"; }
    void paint(PaintInfo& paintInfo, int parentX, int parentY);
    
    virtual QMatrix localTransform() const { return m_transform; }
    virtual void setLocalTransform(const QMatrix& transform) { m_transform = transform; }
    
    bool nodeAtPoint(NodeInfo& info, int _x, int _y, int _tx, int _ty, HitTestAction hitTestAction);

 private:
    QMatrix translationForAttributes();
    QMatrix m_transform;
};

#endif

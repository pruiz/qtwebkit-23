/**
 * This file is part of the HTML widget for KDE.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003 Apple Computer, Inc.
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
#include <config.h>
#include <klocale.h>

#include <kdebug.h>

#include "rendering/render_applet.h"
#include "rendering/render_canvas.h"
#include "xml/dom_docimpl.h"
#include "Frame.h"

#ifndef Q_WS_QWS // We don't have Java in Qt Embedded

#include "java/kjavaappletwidget.h"
#include "html/html_objectimpl.h"

using namespace khtml;
using namespace DOM;
using namespace HTMLNames;

RenderApplet::RenderApplet(HTMLElementImpl *applet, const QMap<QString, QString> &args )
    : RenderWidget(applet), m_args(args)
{
    // init RenderObject attributes
    setInline(true);
}

RenderApplet::~RenderApplet()
{
}

int RenderApplet::intrinsicWidth() const
{
    int rval = 150;

    if( m_widget )
        rval = ((KJavaAppletWidget*)(m_widget))->sizeHint().width();

    return rval > 10 ? rval : 50;
}

int RenderApplet::intrinsicHeight() const
{
    int rval = 150;

    if( m_widget )
        rval = m_widget->sizeHint().height();

    return rval > 10 ? rval : 50;
}

void RenderApplet::createWidgetIfNecessary()
{
    if (!m_widget) {
        if (static_cast<HTMLAppletElementImpl*>(element())->allParamsAvailable()) {
            // FIXME: Java applets can't be resized (this is a bug in Apple's Java implementation).  In order to work around
            // this problem, we will simply use fixed widths/heights from the style system when we can, since the widget might
            // not have an accurate m_width/m_height.
            int width = style()->width().isFixed() ? style()->width().value : 
                        m_width - borderLeft() - borderRight() - paddingLeft() - paddingRight();
            int height = style()->height().isFixed() ? style()->height().value :
                         m_height - borderTop() - borderBottom() - paddingTop() - paddingBottom();
            NodeImpl *child = element()->firstChild();
            while (child) {
                if (child->hasTagName(paramTag)) {
                    HTMLParamElementImpl *p = static_cast<HTMLParamElementImpl *>(child);
                    m_args.insert(p->name().qstring(), p->value().qstring());
                }
                child = child->nextSibling();
            }
        
            setQWidget(new KJavaAppletWidget(QSize(width, height), element()->getDocument()->frame(), m_args));
        }
    }
}

void RenderApplet::layout()
{
    //kdDebug(6100) << "RenderApplet::layout" << endl;

    KHTMLAssert( needsLayout() );
    KHTMLAssert( minMaxKnown() );

    calcWidth();
    calcHeight();

    KJavaAppletWidget *tmp = static_cast<KJavaAppletWidget*>(m_widget);
    // The applet's QWidget gets created lazily upon first layout.
    if (!tmp)
        createWidgetIfNecessary();
    setNeedsLayout(false);
}


RenderEmptyApplet::RenderEmptyApplet(DOM::NodeImpl* node)
  : RenderWidget(node)
{
    // init RenderObject attributes
    setInline(true);

    // FIXME: Figure out how to handle this.
}

int RenderEmptyApplet::intrinsicWidth() const
{
    return (m_widget ? m_widget->sizeHint().width() : 150);
}

int RenderEmptyApplet::intrinsicHeight() const
{
    return (m_widget ? m_widget->sizeHint().height() : 150);
}

void RenderEmptyApplet::layout()
{
    KHTMLAssert( needsLayout() );
    KHTMLAssert( minMaxKnown() );

    calcWidth();
    calcHeight();

    // updateWidgetPositions will size the widget, so we don't need to do that here.
    
    setNeedsLayout(false);
}
#endif

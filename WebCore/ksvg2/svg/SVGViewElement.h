/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef SVGViewElement_h
#define SVGViewElement_h
#if ENABLE(SVG)

#include "SVGStyledElement.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGFitToViewBox.h"
#include "SVGZoomAndPan.h"

namespace WebCore
{
    class SVGStringList;
    class SVGViewElement : public SVGStyledElement,
                               public SVGExternalResourcesRequired,
                               public SVGFitToViewBox,
                               public SVGZoomAndPan
    {
    public:
        SVGViewElement(const QualifiedName&, Document*);
        virtual ~SVGViewElement();

        virtual void parseMappedAttribute(MappedAttribute *attr);

        // 'SVGViewElement' functions
        SVGStringList *viewTarget() const;

        virtual bool rendererIsNeeded(RenderStyle*) { return false; }

    protected:
        virtual const SVGElement* contextElement() const { return this; }

    private:
        mutable RefPtr<SVGStringList> m_viewTarget;

        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGExternalResourcesRequired, bool, ExternalResourcesRequired, externalResourcesRequired)
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGFitToViewBox, FloatRect, ViewBox, viewBox)
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGFitToViewBox, SVGPreserveAspectRatio*, PreserveAspectRatio, preserveAspectRatio)
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet

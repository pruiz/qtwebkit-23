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

#ifndef KSVG_SVGFilterPrimitiveStandardAttributesImpl_H
#define KSVG_SVGFilterPrimitiveStandardAttributesImpl_H
#ifdef SVG_SUPPORT

#include "SVGStyledElement.h"

namespace WebCore {
    class KCanvasFilterEffect;
    class SVGLength;

    class SVGFilterPrimitiveStandardAttributes : public SVGStyledElement
    {
    public:
        SVGFilterPrimitiveStandardAttributes(const QualifiedName&, Document*);
        virtual ~SVGFilterPrimitiveStandardAttributes();
        
        virtual bool isFilterEffect() const { return true; }

        // 'SVGFilterPrimitiveStandardAttributes' functions
        virtual void parseMappedAttribute(MappedAttribute *attr);

        virtual KCanvasFilterEffect *filterEffect() const = 0;

    protected:
        void setStandardAttributes(KCanvasFilterEffect *filterEffect) const;

    protected:
        virtual const SVGElement* contextElement() const { return this; }

    private:
        ANIMATED_PROPERTY_DECLARATIONS(SVGFilterPrimitiveStandardAttributes, SVGLength*, RefPtr<SVGLength>, X, x)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFilterPrimitiveStandardAttributes, SVGLength*, RefPtr<SVGLength>, Y, y)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFilterPrimitiveStandardAttributes, SVGLength*, RefPtr<SVGLength>, Width, width)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFilterPrimitiveStandardAttributes, SVGLength*, RefPtr<SVGLength>, Height, height)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFilterPrimitiveStandardAttributes, String, String, Result, result)
    };

} // namespace WebCore

#endif // SVG_SUPPORT
#endif

// vim:ts=4:noet

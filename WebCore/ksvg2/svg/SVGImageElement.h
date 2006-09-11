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

#ifndef KSVG_SVGImageElementImpl_H
#define KSVG_SVGImageElementImpl_H
#ifdef SVG_SUPPORT

#include "SVGTests.h"
#include "SVGImageLoader.h"
#include "SVGLangSpace.h"
#include "SVGURIReference.h"
#include "SVGStyledTransformableElement.h"
#include "SVGExternalResourcesRequired.h"

namespace WebCore
{
    class SVGPreserveAspectRatio;
    class SVGLength;
    class SVGDocument;

    class SVGImageElement : public SVGStyledTransformableElement,
                                public SVGTests,
                                public SVGLangSpace,
                                public SVGExternalResourcesRequired,
                                public SVGURIReference
    {
    public:
        SVGImageElement(const QualifiedName&, Document*);
        virtual ~SVGImageElement();
        
        virtual bool isValid() const { return SVGTests::isValid(); }

        // 'SVGImageElement' functions
        virtual void parseMappedAttribute(MappedAttribute *attr);
        virtual void attach();

        virtual bool rendererIsNeeded(RenderStyle *style) { return StyledElement::rendererIsNeeded(style); }
        virtual RenderObject *createRenderer(RenderArena *arena, RenderStyle *style);
        
    protected:
        virtual bool haveLoadedRequiredResources();

    protected:
        virtual const SVGElement* contextElement() const { return this; }

    private:
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGExternalResourcesRequired, bool, ExternalResourcesRequired, externalResourcesRequired) 
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGURIReference, String, Href, href)

        ANIMATED_PROPERTY_DECLARATIONS(SVGImageElement, SVGLength*, RefPtr<SVGLength>, X, x)
        ANIMATED_PROPERTY_DECLARATIONS(SVGImageElement, SVGLength*, RefPtr<SVGLength>, Y, y)
        ANIMATED_PROPERTY_DECLARATIONS(SVGImageElement, SVGLength*, RefPtr<SVGLength>, Width, width)
        ANIMATED_PROPERTY_DECLARATIONS(SVGImageElement, SVGLength*, RefPtr<SVGLength>, Height, height)
        ANIMATED_PROPERTY_DECLARATIONS(SVGImageElement, SVGPreserveAspectRatio*, RefPtr<SVGPreserveAspectRatio>, PreserveAspectRatio, preserveAspectRatio)

        SVGImageLoader m_imageLoader;
    };

} // namespace WebCore

#endif // SVG_SUPPORT
#endif

// vim:ts=4:noet

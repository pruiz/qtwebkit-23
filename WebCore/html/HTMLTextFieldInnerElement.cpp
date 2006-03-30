/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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
#include "HTMLTextFieldInnerElement.h"

#include "BeforeTextInsertedEvent.h"
#include "EventNames.h"
#include "dom2_eventsimpl.h"
#include "HTMLDocument.h"
#include "HTMLInputElement.h"
#include "HTMLNames.h"
#include "RenderObject.h"
#include "RenderTextField.h"

namespace WebCore {

using namespace HTMLNames;
using namespace EventNames;

HTMLTextFieldInnerElement::HTMLTextFieldInnerElement(Document* doc, Node* shadowParent)
    : HTMLDivElement(doc), m_shadowParent(shadowParent)
{
}

void HTMLTextFieldInnerElement::defaultEventHandler(Event* evt)
{
    // FIXME:  In the future, we should add a way to have default event listeners.  Then we would add one to the text field's inner div, and we wouldn't need this subclass.
    if (shadowParentNode() && shadowParentNode()->hasTagName(inputTag) && shadowParentNode()->renderer() && shadowParentNode()->renderer()->isTextField()) {
        if (evt->isBeforeTextInsertedEvent())
            static_cast<HTMLInputElement*>(shadowParentNode())->defaultEventHandler(evt);

        if (evt->type() == khtmlEditableContentChangedEvent) {
            // FIXME:  When other text fields switch to the Non-NSView implementation, we may beed to add them here.
            if (static_cast<HTMLInputElement*>(shadowParentNode())->inputType() == HTMLInputElement::TEXT) 
                static_cast<RenderTextField*>(shadowParentNode()->renderer())->subtreeHasChanged();
        }
    }
    HTMLDivElement::defaultEventHandler(evt);
}

}

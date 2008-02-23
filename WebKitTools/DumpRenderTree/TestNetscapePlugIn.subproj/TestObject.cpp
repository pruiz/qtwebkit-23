/*
 * Copyright (C) 2007 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "TestObject.h"
#include "PluginObject.h"

static bool testEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count);
static bool testHasProperty(NPObject *obj, NPIdentifier name);
static NPObject *testAllocate(NPP npp, NPClass *theClass);
static void testDeallocate(NPObject *obj);

static NPClass testClass = { 
    NP_CLASS_STRUCT_VERSION,
    testAllocate, 
    testDeallocate, 
    0,
    0,
    0,
    0,
    testHasProperty,
    0,
    0,
    0,
    testEnumerate
};

NPClass *getTestClass(void)
{
    return &testClass;
}

static bool identifiersInitialized = false;

#define NUM_TEST_IDENTIFIERS 2

static NPIdentifier testIdentifiers[NUM_TEST_IDENTIFIERS];
static const NPUTF8 *testIdentifierNames[NUM_TEST_IDENTIFIERS] = {
    "foo",
    "bar"
};

static void initializeIdentifiers(void)
{
    browser->getstringidentifiers(testIdentifierNames, NUM_TEST_IDENTIFIERS, testIdentifiers);
}

static NPObject *testAllocate(NPP npp, NPClass *theClass)
{
    NPObject *newInstance = (NPObject*)malloc(sizeof(NPObject));
    
    if (!identifiersInitialized) {
        identifiersInitialized = true;
        initializeIdentifiers();
    }
    
    return newInstance;
}

static void testDeallocate(NPObject *obj) 
{
    free(obj);
}

static bool testHasProperty(NPObject *obj, NPIdentifier name)
{
    for (unsigned i = 0; i < NUM_TEST_IDENTIFIERS; i++) {
        if (testIdentifiers[i] == name)
            return true;
    }
    
    return false;
}


static bool testEnumerate(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
    *count = NUM_TEST_IDENTIFIERS;
    
    *value = (NPIdentifier*)browser->memalloc(NUM_TEST_IDENTIFIERS * sizeof(NPIdentifier));
    memcpy(*value, testIdentifiers, sizeof(NPIdentifier) * NUM_TEST_IDENTIFIERS);
    
    return true;
}



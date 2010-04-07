/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
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
#include "AuthenticationCF.h"

#include "AuthenticationChallenge.h"
#include "AuthenticationClient.h"
#include "Credential.h"
#include "ProtectionSpace.h"

#include <CFNetwork/CFURLAuthChallengePriv.h>
#include <CFNetwork/CFURLCredentialPriv.h>
#include <CFNetwork/CFURLProtectionSpacePriv.h>

namespace WebCore {

AuthenticationChallenge::AuthenticationChallenge(const ProtectionSpace& protectionSpace,
                                                 const Credential& proposedCredential,
                                                 unsigned previousFailureCount,
                                                 const ResourceResponse& response,
                                                 const ResourceError& error)
    : AuthenticationChallengeBase(protectionSpace,
                                  proposedCredential,
                                  previousFailureCount,
                                  response,
                                  error)
{
}

AuthenticationChallenge::AuthenticationChallenge(CFURLAuthChallengeRef cfChallenge,
                                                 AuthenticationClient* authenticationClient)
    : AuthenticationChallengeBase(core(CFURLAuthChallengeGetProtectionSpace(cfChallenge)),
                                  core(CFURLAuthChallengeGetProposedCredential(cfChallenge)),
                                  CFURLAuthChallengeGetPreviousFailureCount(cfChallenge),
                                  (CFURLResponseRef)CFURLAuthChallengeGetFailureResponse(cfChallenge),
                                  CFURLAuthChallengeGetError(cfChallenge))
    , m_authenticationClient(authenticationClient)
    , m_cfChallenge(cfChallenge)
{
}

bool AuthenticationChallenge::platformCompare(const AuthenticationChallenge& a, const AuthenticationChallenge& b)
{
    if (a.authenticationClient() != b.authenticationClient())
        return false;

    if (a.cfURLAuthChallengeRef() != b.cfURLAuthChallengeRef())
        return false;
        
    return true;
}

CFURLAuthChallengeRef createCF(const AuthenticationChallenge& coreChallenge)
{  
    CFURLProtectionSpaceRef protectionSpace = createCF(coreChallenge.protectionSpace());
    CFURLCredentialRef credential = createCF(coreChallenge.proposedCredential());
    
    CFURLAuthChallengeRef result = CFURLAuthChallengeCreate(0, protectionSpace, credential,
                                        coreChallenge.previousFailureCount(),
                                        coreChallenge.failureResponse().cfURLResponse(),
                                        coreChallenge.error());
    CFRelease(protectionSpace);
    CFRelease(credential);
    return result;
}

CFURLCredentialRef createCF(const Credential& coreCredential)
{
    CFURLCredentialPersistence persistence = kCFURLCredentialPersistenceNone;
    switch (coreCredential.persistence()) {
    case CredentialPersistenceNone:
        break;
    case CredentialPersistenceForSession:
        persistence = kCFURLCredentialPersistenceForSession;
        break;
    case CredentialPersistencePermanent:
        persistence = kCFURLCredentialPersistencePermanent;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
    
    CFStringRef user = coreCredential.user().createCFString();
    CFStringRef password = coreCredential.password().createCFString();
    CFURLCredentialRef result = CFURLCredentialCreate(0, user, password, 0, persistence);
    CFRelease(user);
    CFRelease(password);

    return result;
}

CFURLProtectionSpaceRef createCF(const ProtectionSpace& coreSpace)
{
    CFURLProtectionSpaceServerType serverType = kCFURLProtectionSpaceServerHTTP;
    switch (coreSpace.serverType()) {
    case ProtectionSpaceServerHTTP:
        serverType = kCFURLProtectionSpaceServerHTTP;
        break;
    case ProtectionSpaceServerHTTPS:
        serverType = kCFURLProtectionSpaceServerHTTPS;
        break;
    case ProtectionSpaceServerFTP:
        serverType = kCFURLProtectionSpaceServerFTP;
        break;
    case ProtectionSpaceServerFTPS:
        serverType = kCFURLProtectionSpaceServerFTPS;
        break;
    case ProtectionSpaceProxyHTTP:
        serverType = kCFURLProtectionSpaceProxyHTTP;
        break;
    case ProtectionSpaceProxyHTTPS:
        serverType = kCFURLProtectionSpaceProxyHTTPS;
        break;
    case ProtectionSpaceProxyFTP:
        serverType = kCFURLProtectionSpaceProxyFTP;
        break;
    case ProtectionSpaceProxySOCKS:
        serverType = kCFURLProtectionSpaceProxySOCKS;
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    CFURLProtectionSpaceAuthenticationScheme scheme = kCFURLProtectionSpaceAuthenticationSchemeDefault;
    switch (coreSpace.authenticationScheme()) {
    case ProtectionSpaceAuthenticationSchemeDefault:
        scheme = kCFURLProtectionSpaceAuthenticationSchemeDefault;
        break;
    case ProtectionSpaceAuthenticationSchemeHTTPBasic:
        scheme = kCFURLProtectionSpaceAuthenticationSchemeHTTPBasic;
        break;
    case ProtectionSpaceAuthenticationSchemeHTTPDigest:
        scheme = kCFURLProtectionSpaceAuthenticationSchemeHTTPDigest;
        break;
    case ProtectionSpaceAuthenticationSchemeHTMLForm:
        scheme = kCFURLProtectionSpaceAuthenticationSchemeHTMLForm;
        break;
    case ProtectionSpaceAuthenticationSchemeNTLM:
        scheme = kCFURLProtectionSpaceAuthenticationSchemeNTLM;
        break;
    case ProtectionSpaceAuthenticationSchemeNegotiate:
        scheme = kCFURLProtectionSpaceAuthenticationSchemeNegotiate;
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    CFStringRef host = coreSpace.host().createCFString();
    CFStringRef realm = coreSpace.realm().createCFString();
    CFURLProtectionSpaceRef result = CFURLProtectionSpaceCreate(0, host, coreSpace.port(), serverType, realm, scheme);
    CFRelease(host);
    CFRelease(realm);
    
    return result;
}

Credential core(CFURLCredentialRef cfCredential)
{
    if (!cfCredential)
        return Credential();

    CredentialPersistence persistence = CredentialPersistenceNone;
    switch (CFURLCredentialGetPersistence(cfCredential)) {
    case kCFURLCredentialPersistenceNone:
        break;
    case kCFURLCredentialPersistenceForSession:
        persistence = CredentialPersistenceForSession;
        break;
    case kCFURLCredentialPersistencePermanent:
        persistence = CredentialPersistencePermanent;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
    
    return Credential(CFURLCredentialGetUsername(cfCredential), CFURLCredentialCopyPassword(cfCredential), persistence);
}

ProtectionSpace core(CFURLProtectionSpaceRef cfSpace)
{
    ProtectionSpaceServerType serverType = ProtectionSpaceServerHTTP;
    
    switch (CFURLProtectionSpaceGetServerType(cfSpace)) {
    case kCFURLProtectionSpaceServerHTTP:
        break;
    case kCFURLProtectionSpaceServerHTTPS:
        serverType = ProtectionSpaceServerHTTPS;
        break;
    case kCFURLProtectionSpaceServerFTP:
        serverType = ProtectionSpaceServerFTP;
        break;
    case kCFURLProtectionSpaceServerFTPS:
        serverType = ProtectionSpaceServerFTPS;
        break;
    case kCFURLProtectionSpaceProxyHTTP:
        serverType = ProtectionSpaceProxyHTTP;
        break;
    case kCFURLProtectionSpaceProxyHTTPS:
        serverType = ProtectionSpaceProxyHTTPS;
        break;
    case kCFURLProtectionSpaceProxyFTP:
        serverType = ProtectionSpaceProxyFTP;
        break;
    case kCFURLProtectionSpaceProxySOCKS:
        serverType = ProtectionSpaceProxySOCKS;
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    ProtectionSpaceAuthenticationScheme scheme = ProtectionSpaceAuthenticationSchemeDefault;
    
    switch (CFURLProtectionSpaceGetAuthenticationScheme(cfSpace)) {
    case kCFURLProtectionSpaceAuthenticationSchemeDefault:
        scheme = ProtectionSpaceAuthenticationSchemeDefault;
        break;
    case kCFURLProtectionSpaceAuthenticationSchemeHTTPBasic:
        scheme = ProtectionSpaceAuthenticationSchemeHTTPBasic;
        break;
    case kCFURLProtectionSpaceAuthenticationSchemeHTTPDigest:
        scheme = ProtectionSpaceAuthenticationSchemeHTTPDigest;
        break;
    case kCFURLProtectionSpaceAuthenticationSchemeHTMLForm:
        scheme = ProtectionSpaceAuthenticationSchemeHTMLForm;
        break;
    case kCFURLProtectionSpaceAuthenticationSchemeNTLM:
        scheme = ProtectionSpaceAuthenticationSchemeNTLM;
        break;
    case kCFURLProtectionSpaceAuthenticationSchemeNegotiate:
        scheme = ProtectionSpaceAuthenticationSchemeNegotiate;
        break;
    default:
        scheme = ProtectionSpaceAuthenticationSchemeUnknown;
        ASSERT_NOT_REACHED();
    }
        
    return ProtectionSpace(CFURLProtectionSpaceGetHost(cfSpace), 
                           CFURLProtectionSpaceGetPort(cfSpace),
                           serverType,
                           CFURLProtectionSpaceGetRealm(cfSpace),
                           scheme);
}

};

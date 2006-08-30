# 
# KDOM IDL parser
#
# Copyright (C) 2005 Nikolas Zimmermann <wildfox@kde.org>
# Copyright (C) 2006 Anders Carlsson <andersca@mac.com> 
# Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
# Copyright (C) 2006 Apple Computer, Inc.
#
# This file is part of the KDE project
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
# 
# You should have received a copy of the GNU Library General Public License
# aint with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#

package CodeGeneratorObjC;

use File::stat;

my $module = "";
my $outputDir = "";
my %implIncludes = ();
my %headerForwardDeclarations = ();
my %headerForwardDeclarationsForProtocols = ();

my $exceptionInit = "WebCore::ExceptionCode ec = 0;";
my $exceptionRaiseOnError = "raiseOnDOMError(ec);";

# Default Licence Templates
my $headerLicenceTemplate = << "EOF";
/*
 * Copyright (C) 2004, 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Samuel Weinig <sam.weinig\@gmail.com>
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
EOF

my $implementationLicenceTemplate = << "EOF";
/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

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
EOF

# Default constructor
sub new
{
    my $object = shift;
    my $reference = { };

    $codeGenerator = shift;
    $outputDir = shift;

    bless($reference, $object);
    return $reference;
}

sub finish
{
    my $object = shift;

    # Commit changes!
    $object->WriteData();
}

# Params: 'domClass' struct
sub GenerateInterface
{
    my $object = shift;
    my $dataNode = shift;

    $object->RemoveExcludedAttributesAndFunctions($dataNode);

    # Start actual generation..
    $object->GenerateHeader($dataNode);
    $object->GenerateImplementation($dataNode);

    my $name = $dataNode->name;

    # Open files for writing...
    my $headerFileName = "$outputDir/DOM$name.h";
    my $implFileName = "$outputDir/DOM$name.mm";

    open($IMPL, ">$implFileName") or die "Couldn't open file $implFileName";
    open($HEADER, ">$headerFileName") or die "Couldn't open file $headerFileName";
}

# Params: 'idlDocument' struct
sub GenerateModule
{
    my $object = shift;
    my $dataNode = shift;  
    
    $module = $dataNode->module;    
}

sub RemoveExcludedAttributesAndFunctions
{
    my $object = shift;
    my $dataNode = shift;

    my $i = 0;

    while ($i < @{$dataNode->attributes}) {
        my $lang = ${$dataNode->attributes}[$i]->signature->extendedAttributes->{"Exclude"};
        if ($lang and $lang eq "ObjC") {
            splice(@{$dataNode->attributes}, $i, 1);
        } else {
            $i++;
        }
    }

    $i = 0;
    while ($i < @{$dataNode->functions}) {
        my $lang = ${$dataNode->functions}[$i]->signature->extendedAttributes->{"Exclude"};
        if ($lang and $lang eq "ObjC") {
            splice(@{$dataNode->functions}, $i, 1);
        } else {
            $i++;
        }
    }
}

sub GetClassName
{
    my $name = $codeGenerator->StripModule(shift);
    
    # special cases
    if ($name eq "boolean") {
        return "BOOL";
    } elsif ($name eq "unsigned long") {
        return "unsigned";
    } elsif ($name eq "long") {
        return "int";
    } elsif ($name eq "DOMString") {
        return "NSString";
    } elsif ($name eq "DOMWindow") {
        return "DOMAbstractView";
    } elsif ($name eq "XPathNSResolver") {
        return "id <DOMXPathNSResolver>";
    } elsif ($name eq "unsigned short" 
             or $name eq "float"
             or $name eq "void"
             or $name eq "DOMImplementation") {
        return $name;
    }

    # Default, assume objective-c type has the same type name as
    # idl type prefixed with "DOM".
    return "DOM" . $name;
}

sub GetImplClassName
{
    my $name = $codeGenerator->StripModule(shift);
    
    # special cases
    if ($name eq "DOMImplementation") {
        return "WebCore::DOMImplementationFront";
    }

    return "WebCore::" . $name;
}

sub GetParentImplClassName
{
    my $dataNode = shift;

    if (@{$dataNode->parents} eq 0) {
        return "Object";
    }

    my $parent = $codeGenerator->StripModule($dataNode->parents(0));
    
    # special cases
    if ($parent eq "EventTargetNode") {
        $parent = "Node";
    } elsif ($parent eq "HTMLCollection") {
        $parent = "Object";
    }

    return $parent;
}

sub GetObjCType
{
    my $name = GetClassName(shift);

    if ($codeGenerator->IsPrimitiveType($name)
            or $name eq "BOOL"
            or $name eq "unsigned"
            or $name eq "int"
            or $name eq "id <DOMXPathNSResolver>") {
        return $name;
    }

    # Default, return type as a pointer.
    return "$name *";
}

sub GetObjCTypeMaker
{
    my $type = $codeGenerator->StripModule(shift);

    if ($codeGenerator->IsPrimitiveType($type) or $type eq "DOMString") {
        return "";
    }

    if ($type eq "DOMRGBColor") {
        return "_RGBColorWithRGB";
    }

    my $typeMaker = "";

    if ($type eq "HTMLCollection") {
        $typeMaker = "collection";
    } elsif ($type eq "HTMLFormElement") {
        $typeMaker = "formElement";
    } elsif ($type eq "HTMLDocument") {
        $typeMaker = "HTMLDocument";
    } elsif ($type eq "CSSStyleDeclaration") {
        $typeMaker = "styleDeclaration";
    } elsif ($type eq "CSSStyleSheet") {
        $typeMaker = "CSSStyleSheet";
    } elsif ($type eq "DOMImplementation") {
        $typeMaker = "DOMImplementation";
    } elsif ($type eq "CDATASection") {
        $typeMaker = "CDATASection";
    } elsif ($type eq "DOMWindow") {
        $typeMaker = "abstractView";
    } elsif ($type eq "XPathResult") {
        $typeMaker = "xpathResult";
    } elsif ($type eq "XPathNSResolver") {
        $typeMaker = "xpathNSResolver";
    } elsif ($type eq "XPathExpression") {
        $typeMaker = "xpathExpression";
    } else {
        $typeMaker = lcfirst($type);
    }

    # put into the form "_fooBarWith" for type FooBar.
    $typeMaker = "_" . $typeMaker . "With";
    return $typeMaker;
}

sub AddForwardDeclarationsForType
{
    my $type = $codeGenerator->StripModule(shift);
    
    if ($codeGenerator->IsPrimitiveType($type) or $type eq "DOMString") {
        return;
    }
    
    if ($type eq "DOMImplementation") {
        $headerForwardDeclarations{"$type"} = 1;
        return;
    }

    if ($type eq "DOMWindow") {
        $headerForwardDeclarations{"DOMAbstractView"} = 1;
        return;
    }

    if ($type eq "XPathNSResolver") {
        # Only one protocol so far.
        $headerForwardDeclarationsForProtocols{"DOMXPathNSResolver"} = 1;
        return;
    }

    $headerForwardDeclarations{"DOM$type"} = 1;
}

sub AddIncludesForType
{
    my $type = $codeGenerator->StripModule(shift);
    
    if ($codeGenerator->IsPrimitiveType($type)) {
        return;
    }

    if ($type eq "DOMString") {
        $implIncludes{"PlatformString.h"} = 1;
        return;
    }

    # Temp DOMCSS.h
    if ($type eq "Counter"
            or $type eq "MediaList"
            or $type eq "CSSStyleSheet") {
        $implIncludes{"DOMCSS.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        return;
    }
    if ($type eq "CSSStyleDeclaration") {
        $implIncludes{"DOMCSS.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        $implIncludes{"CSSMutableStyleDeclaration.h"} = 1;
        return;
    }
    if ($type eq "RGBColor" or $type eq "Rect") {
        $implIncludes{"DOMCSS.h"} = 1;
        return;
    }

    # Temp DOMHTML.h
    if ($type eq "HTMLDocument") {
        $implIncludes{"DOMHTML.h"} = 1;
        $implIncludes{"DOMHTMLInternal.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        return;
    }

    # Temp DOMEvents.h
    if ($type eq "Event") {
        $implIncludes{"DOMEvents.h"} = 1;
        $implIncludes{"DOMEventsInternal.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        return;
    }

    # Temp DOMStyleSheets.h
    if ($type eq "StyleSheetList") {
        $implIncludes{"DOMStyleSheets.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        return;
    }
    
    # Temp DOMViews.h
    if ($type eq "DOMWindow") {
        $implIncludes{"DOMViews.h"} = 1;
        $implIncludes{"DOMViewsInternal.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        return;
    }
    
    # Temp DOMXPath.h
    if ($type eq "XPathExpression"
            or $type eq "XPathNSResolver"
            or $type eq "XPathResult") {
        $implIncludes{"DOMXPath.h"} = 1;
        $implIncludes{"DOMXPathInternal.h"} = 1;
        $implIncludes{"$type.h"} = 1;
        return;
    }

    # Temp DOMImplementationFront.h
    if ($type eq "DOMImplementation") {
        $implIncludes{"DOMImplementationFront.h"} = 1;
    }

    # Add type specific internal types.
    $implIncludes{"DOMHTMLInternal.h"} = 1 if ($type =~ /^HTML/);

    # Default, include the same named file (the implementation) and the same name prefixed with "DOM". 
    $implIncludes{"$type.h"} = 1;
    $implIncludes{"DOM$type.h"} = 1;
}

sub GenerateHeader
{
    my $object = shift;
    my $dataNode = shift;

    # Make sure that we don't have more than one parent.
    if (@{$dataNode->parents} > 1) {
        die "A class can't have more than one parent.";
    }

    my $interfaceName = $dataNode->name;
    my $className = GetClassName($interfaceName);
    my $parentClassName = "DOM" . GetParentImplClassName($dataNode);

    my $numConstants = @{$dataNode->constants};
    my $numAttributes = @{$dataNode->attributes};
    my $numFunctions = @{$dataNode->functions};

    # - Add default header template
    @headerContentHeader = split("\r", $headerLicenceTemplate);

    # - INCLUDES -
    push(@headerContentHeader, "\n#import <WebCore/$parentClassName.h> // parent class\n\n");

    # - Add constants.
    if ($numConstants > 0) {
        my @headerConstants = ();
        foreach my $constant (@{$dataNode->constants}) {

            my $constantName = $constant->name;
            my $constantValue = $constant->value;
            my $output = "    DOM_" . $constantName . " = " . $constantValue;
            
            push(@headerConstants, $output);
        }

        my $combinedConstants = join(",\n", @headerConstants);

        # FIXME: the formatting of the enums should line up the equal signs.
        push(@headerContent, "\n// Constants\n");
        push(@headerContent, "enum {\n");
        push(@headerContent, $combinedConstants);
        push(@headerContent, "\n};\n");        
    }
    
    my %hashOfCatagories = ();
    
    # - Begin @interface 
    push(@headerContent, "\n\@interface $className : $parentClassName\n");

    # - Add attribute getters/setters.
    if ($numAttributes > 0) {
        my @headerAttributes = ();

        foreach (@{$dataNode->attributes}) {
            my $attribute = $_;
            
            AddForwardDeclarationsForType($attribute->signature->type);

            my $attributeName = $attribute->signature->name;
            if ($attributeName eq "id") {
                # Special case attribute id to be idName to avoid Obj-C nameing conflict.
                $attributeName .= "Name";
            } elsif ($attributeName eq "frame") {
                # Special case attribute frame to be frameBorders.
                $attributeName .= "Borders";
            }

            my $attributeType = GetObjCType($attribute->signature->type);
            my $attributeIsReadonly = ($attribute->type =~ /^readonly/);
            my $catagory = $attribute->signature->extendedAttributes->{"ObjCCatagory"};

            if ($ENV{"MACOSX_DEPLOYMENT_TARGET"} >= 10.5) {
                my $property = "\@property" . ($attributeIsReadonly ? "(readonly)" : "") . " " . $attributeType . ($attributeType =~ /\*$/ ? "" : " ") . $attributeName . ";\n";

                if ($catagory) {
                    push(@{ $hashOfCatagories{$catagory} }, $property);
                } else {
                    push(@headerAttributes, $property);
                }
            } else {
                # - GETTER
                my $getter = "- (" . $attributeType . ")" . $attributeName . ";\n";

                if ($catagory) {
                    push(@{ $hashOfCatagories{$catagory} }, $getter);
                } else {
                    push(@headerAttributes, $getter);
                }

                # - SETTER
                if (!$attributeIsReadonly) {
                    my $setter = "- (void)set" . ucfirst($attributeName) . ":(" . $attributeType . ")new" . ucfirst($attributeName) . ";\n";
                    
                    if ($catagory) {
                        push(@{ $hashOfCatagories{$catagory} }, $setter);
                    } else {
                        push(@headerAttributes, $setter);
                    }
                }
            }
        }

        if (@headerAttributes > 0) {
            push(@headerContent, @headerAttributes);
        }
    }

    # - Add functions.
    if ($numFunctions > 0) {
        my @headerFunctions = ();

        foreach (@{$dataNode->functions}) {
            my $function = $_;

            AddForwardDeclarationsForType($function->signature->type);

            my $functionName = $function->signature->name;
            my $returnType = GetObjCType($function->signature->type);
            my $numberOfParameters = @{$function->parameters};
            my $catagory = $function->signature->extendedAttributes->{"ObjCCatagory"};

            my $output = "- ($returnType)$functionName";
            foreach my $param (@{$function->parameters}) {
                my $paramName = $param->name;
                my $paramType = GetObjCType($param->type);
                AddForwardDeclarationsForType($param->type);

                $output .= ":($paramType)$paramName ";
            }
            # remove any trailing spaces.
            $output =~ s/\s+$//;
            $output .= ";\n";
    
            if ($catagory) {
                push(@{ $hashOfCatagories{$catagory} }, $output);
            } else {
                push(@headerFunctions, $output);
            }
        }

        if (@headerFunctions > 0) {
            push(@headerContent, "\n");
            push(@headerContent, @headerFunctions);
        }
    }

    # - End @interface 
    push(@headerContent, "\@end\n");
    
    # Add additional Catagories (if any)
    if (scalar(keys(%hashOfCatagories))) {
        foreach(sort(keys(%hashOfCatagories))) {
            my $catagory = $_;

            # - Begin @interface 
            push(@headerContent, "\n\@interface $className ($catagory)\n");
            
            foreach (@{ $hashOfCatagories{$catagory} }) {
                my $declaration = $_;
                push(@headerContent, $declaration);
            }
            
            # - End @interface
            push(@headerContent, "\@end\n");
        }
    }
}

sub GenerateImplementation
{
    my $object = shift;
    my $dataNode = shift;

    my $interfaceName = $dataNode->name;
    my $className = GetClassName($interfaceName);
    my $implClassName = GetImplClassName($interfaceName);
    my $parentImplClassName = GetParentImplClassName($dataNode);

    my $numAttributes = @{$dataNode->attributes};
    my $numFunctions = @{$dataNode->functions};
    my $hasFunctionsOrAttributes = $numAttributes + $numFunctions;

    # - Add default header template.
    @implContentHeader = split("\r", $implementationLicenceTemplate);

    # - INCLUDES -
    push(@implContentHeader, "\n#import \"config.h\"\n");
    push(@implContentHeader, "#import \"$className.h\"\n\n");

    if ($hasFunctionsOrAttributes) {
        # NEEDED for DOM_CAST
        push(@implContentHeader, "#import \"DOMInternal.h\" // needed for DOM_cast<>\n");
        
        # include module dependant internal interfaces.
        if ($module eq "html") {
            # HTML module internal interfaces
            push(@implContentHeader, "#import \"DOMHTMLInternal.h\"\n");
        } elsif ($module eq "css") {
            # CSS module internal interfaces
            push(@implContentHeader, "#import \"DOMCSSInternal.h\"\n");
        } elsif ($module eq "events") {
            # CSS module internal interfaces
            push(@implContentHeader, "#import \"DOMEventsInternal.h\"\n");
        } elsif ($module eq "xpath") {
            # CSS module internal interfaces
            push(@implContentHeader, "#import \"DOMXPathInternal.h\"\n");
        }

        # include Implementation class
        push(@implContentHeader, "#import \"$interfaceName.h\" // implementation class\n");
        if ($interfaceName eq "DOMImplementation") {
            # FIXME: needed until we can remove DOMImplementationFront
            push(@implContentHeader, "#import \"DOMImplementationFront.h\"\n");
        }
    }

    @implContent = ();

    # START implementation
    push(@implContent, "\n\@implementation $className\n\n");

    # ADD INTERNAL CASTING MACRO
    my $implementation = "IMPL_" . uc($interfaceName);

    if ($hasFunctionsOrAttributes) {
        if ($parentImplClassName eq "Object") {
            # Only generate 'dealloc' and 'finalize' methods for direct subclasses of DOMObject.

            push(@implContent, "#define $implementation DOM_cast<$implClassName *>(_internal)\n\n");

            push(@implContent, "- (void)dealloc\n");
            push(@implContent, "{\n");
            push(@implContent, "    if (_internal)\n");
            push(@implContent, "        $implementation->deref();\n");
            push(@implContent, "    [super dealloc];\n");
            push(@implContent, "}\n\n");

            push(@implContent, "- (void)finalize\n");
            push(@implContent, "{\n");
            push(@implContent, "    if (_internal)\n");
            push(@implContent, "        $implementation->deref();\n");
            push(@implContent, "    [super finalize];\n");
            push(@implContent, "}\n\n");
        } else {
            my $internalBaseType;
            if ($interfaceName =~ /^CSS.+Value$/) {
                $internalBaseType = "WebCore::CSSValue"
            } elsif ($interfaceName =~ /^CSS.+Rule$/) {
                $internalBaseType = "WebCore::CSSRule"
            } else {
                $internalBaseType = "WebCore::Node"
            }

            push(@implContent, "#define $implementation static_cast<$implClassName *>(DOM_cast<$internalBaseType *>(_internal))\n\n");
        }
    }

    # - Attributes
    if ($numAttributes > 0) {
        foreach (@{$dataNode->attributes}) {
            my $attribute = $_;

            AddIncludesForType($attribute->signature->type);

            my $attributeName = $attribute->signature->name;
            my $attributeType = GetObjCType($attribute->signature->type);
            my $attributeIsReadonly = ($attribute->type =~ /^readonly/);

            my $interfaceName = $attributeName;
            if ($attributeName eq "id") {
                # Special case attribute id to be idName to avoid Obj-C nameing conflict.
                $interfaceName .= "Name";
            } elsif ($attributeName eq "frame") {
                # Special case attribute frame to be frameBorders.
                $interfaceName .= "Borders";
            }

            # - GETTER
            my $getterSig = "- ($attributeType)$interfaceName\n";
            
            # Exception handling
            my $hasGetterException = @{$attribute->getterExceptions};
            if ($hasGetterException) {
                die "We should not have any getter exceptions yet!";
            }
            
            my $getterContentHead = "$implementation->$attributeName(";
            my $getterContentTail = ")";

            my $attributeTypeSansPtr = $attributeType;
            $attributeTypeSansPtr =~ s/ \*$//; # Remove trailing " *" from pointer types.
            my $typeMaker = GetObjCTypeMaker($attribute->signature->type);
            
            # Special cases
            if ($attributeTypeSansPtr eq "DOMImplementation") {
                # FIXME: We have to special case DOMImplementation until DOMImplementationFront is removed
                $getterContentHead = "[$attributeTypeSansPtr $typeMaker:implementationFront($implementation";
                $getterContentTail .= "]";
            } elsif ($attributeName =~ /(\w+)DisplayString$/) {
                my $attributeToDisplay = $1;
                $getterContentHead = "$implementation->$attributeToDisplay().replace(\'\\\\\', [self _element]->document()->backslashAsCurrencySymbol()";
                $implIncludes{"Document.h"} = 1;
            } elsif ($typeMaker ne "") {
                # Surround getter with TypeMaker
                $getterContentHead = "[$attributeTypeSansPtr $typeMaker:" . $getterContentHead;
                $getterContentTail .= "]";
            }
            
            if ($attribute->signature->extendedAttributes->{"UsesPassRefPtr"}) {
                $getterContentTail = ").get(" . $getterContentTail;
            }
            
            my $getterContent;
            if ($hasGetterException) {
                $getterContent = $getterContentHead . "ec" . $getterContentTail;
            } else {
                $getterContent = $getterContentHead . $getterContentTail;
            }
            
            push(@implContent, $getterSig);
            push(@implContent, "{\n");
            if ($hasGetterException) {
                # Differentiated between when the return type is a pointer and
                # not for white space issue (ie. Foo *result vs. int result).
                if ($attributeType =~ /\*$/) {
                    $getterContent = $attributeType . "result = " . $getterContent;
                } else {
                    $getterContent = $attributeType . " result = " . $getterContent;
                }

                push(@implContent, "    $exceptionInit\n");
                push(@implContent, "    $getterContent;\n");
                push(@implContent, "    $exceptionRaiseOnError\n");
                push(@implContent, "    return result;\n");
            } else {
                push(@implContent, "    return $getterContent;\n");
            }
            push(@implContent, "}\n\n");

            # - SETTER
            if (!$attributeIsReadonly) {
                # Exception handling
                my $hasSetterException = @{$attribute->setterExceptions};

                $attributeName = "set" . ucfirst($attributeName);
                my $setterName = "set" . ucfirst($interfaceName);
                my $argName = "new" . ucfirst($interfaceName);

                my $setterSig = "- (void)$setterName:($attributeType)$argName\n";

                push(@implContent, $setterSig);
                push(@implContent, "{\n");

                if ($hasSetterException) {
                    # FIXME: asserts exsist in the exsisting bindings, but I am unsure why they are 
                    # there in the first place;
                    push(@implContent, "    ASSERT($argName);\n\n");

                    push(@implContent, "    $exceptionInit\n");
                    push(@implContent, "    $implementation->$attributeName($argName, ec);\n");
                    push(@implContent, "    $exceptionRaiseOnError\n");
                } else {
                    push(@implContent, "    $implementation->$attributeName($argName);\n");
                }

                push(@implContent, "}\n\n");
            }
        }
    }

    # - Functions
    if ($numFunctions > 0) {
        foreach (@{$dataNode->functions}) {
            my $function = $_;

            AddIncludesForType($function->signature->type);

            my $functionName = $function->signature->name;
            my $returnType = GetObjCType($function->signature->type);
            my $hasParameters = @{$function->parameters};
            my $raisesExceptions = @{$function->raisesExceptions};

            my @parameterNames = ();
            my @needsAssert = ();
            my %custom = ();

            my $functionSig = "- ($returnType)$functionName";
            foreach (@{$function->parameters}) {
                my $param = $_;

                my $paramType = GetObjCType($param->type);
                AddIncludesForType($param->type);

                my $paramName = $param->name;

                # FIXME: should move this out into it's own fuction to take care of possible special cases.
                my $idlType = $codeGenerator->StripModule($param->type);
                if ($codeGenerator->IsPrimitiveType($idlType) or $idlType eq "DOMString") {
                    push(@parameterNames, $paramName);
                } elsif ($idlType eq "XPathNSResolver") {
                    my $implGetter = "[nativeResolver _xpathNSResolver]";
                    push(@parameterNames, $implGetter);
                    $needsCustom{"XPathNSResolver"} = $paramName;
                } elsif ($idlType eq "XPathResult") {
                    my $implGetter = "[" . $paramName . " _xpathResult]";
                    push(@parameterNames, $implGetter);
                } else {
                    my $implGetter = "[" . $paramName . " _" . lcfirst($idlType) . "]";
                    push(@parameterNames, $implGetter);
                }

                if (!$param->extendedAttributes->{"IsIndex"}) {
                    push(@needsAssert, "    ASSERT($paramName);\n");
                }

                $functionSig .= ":($paramType)$paramName ";
            }
            # remove any trailing spaces.
            $functionSig =~ s/\s+$//;

            my @functionContent = ();

            if ($returnType eq "void") {
                # Special case 'void' return type.
                my $functionContentHead = "$implementation->$functionName(";
                my $functionContentTail = ");";
                my $content = "";

                if ($hasParameters) {
                    my $params = join(", ", @parameterNames);
                    if ($raisesExceptions) {
                        $content = $functionContentHead . $params . ", ec" . $functionContentTail;
                    } else {
                        $content = $functionContentHead . $params . $functionContentTail;
                    }
                } else {
                    if ($raisesExceptions) {
                        $content = $functionContentHead . "ec" . $functionContentTail;
                    } else {
                        $content = $functionContentHead . $functionContentTail;
                    }
                }
                
                if ($raisesExceptions) {
                    push(@functionContent, "    $exceptionInit\n");
                    push(@functionContent, "    $content\n");
                    push(@functionContent, "    $exceptionRaiseOnError\n");
                } else {
                    push(@functionContent, "    $content\n");
                }
            } else {
                my $functionContentHead = $implementation . "->" . $functionName . "(";
                my $functionContentTail = ")";

                my $typeMaker = GetObjCTypeMaker($function->signature->type);
                unless ($typeMaker eq "") {
                    my $returnTypeClass = "";
                    if ($function->signature->type eq "XPathNSResolver") {
                        # Special case XPathNSResolver
                        $returnTypeClass = "DOMNativeXPathNSResolver";
                    } else {
                        # Remove trailing " *" from pointer types.
                        $returnTypeClass = $returnType;
                        $returnTypeClass =~ s/ \*$//;
                    }

                    # Surround getter with TypeMaker
                    $functionContentHead = "[$returnTypeClass $typeMaker:" . $functionContentHead;
                    $functionContentTail .= "]";
                }

                if ($function->signature->extendedAttributes->{"UsesPassRefPtr"}) {
                    $functionContentTail = ").get(" . $functionContentTail;
                }

                my $content = "";

                if ($hasParameters) {
                    my $params = join(", " , @parameterNames);
                    if ($raisesExceptions) {
                        # A temparary variable is needed.
                        $content = $functionContentHead . $params . ", ec" . $functionContentTail;
                    } else {
                        $content = $functionContentHead . $params . $functionContentTail;
                    }
                } else {
                    if ($raisesExceptions) {
                        # A temparary variable is needed.
                        $content = $functionContentHead . "ec" . $functionContentTail;
                    } else {
                        $content = $functionContentHead . $functionContentTail;
                    }
                }
                
                if ($raisesExceptions) {
                    # Differentiated between when the return type is a pointer and
                    # not for white space issue (ie. Foo *result vs. int result).
                    if ($returnType =~ /\*$/) {
                        $content = $returnType . "result = " . $content;
                    } else {
                        $content = $returnType . " result = " . $content;
                    }
                    
                    push(@functionContent, "    $exceptionInit\n");
                    push(@functionContent, "    $content;\n");
                    push(@functionContent, "    $exceptionRaiseOnError\n");
                    push(@functionContent, "    return result;\n");
                } else {
                    push(@functionContent, "    return $content;\n");
                }
            }

            push(@implContent, "$functionSig\n");
            push(@implContent, "{\n");
            
            # special case the XPathNSResolver
            if (defined $needsCustom{"XPathNSResolver"}) {
                my $paramName = $needsCustom{"XPathNSResolver"};
                push(@implContent, "    if ($paramName && ![$paramName isMemberOfClass:[DOMNativeXPathNSResolver class]])\n");
                push(@implContent, "        [NSException raise:NSGenericException format:\@\"createExpression currently does not work with custom NS resolvers\"];\n");
                push(@implContent, "    DOMNativeXPathNSResolver *nativeResolver = (DOMNativeXPathNSResolver *)$paramName;\n\n");
            }

            push(@implContent, @functionContent);
            push(@implContent, "}\n\n");
            
            # Clear the hash
            %needsCustom = ();
        }
    }

    # END implementation
    push(@implContent, "\@end\n");
}

# Internal helper
sub WriteData
{
    if (defined($IMPL)) {
        # Write content to file.
        print $IMPL @implContentHeader;
        
        foreach my $implInclude (sort keys(%implIncludes)) {
            print $IMPL "#import \"$implInclude\"\n";
        }
        
        print $IMPL @implContent;
        close($IMPL);
        undef($IMPL);
        
        @implHeaderContent = "";
        @implContent = "";    
        %implIncludes = ();
    }

    if (defined($HEADER)) {
        # Write content to file.
        print $HEADER @headerContentHeader;
        
        foreach my $forwardClassDeclaration (sort keys(%headerForwardDeclarations)) {
            print $HEADER "\@class $forwardClassDeclaration;\n";
        }
        
        foreach my $forwardProtocolDeclaration (sort keys(%headerForwardDeclarationsForProtocols)) {
            print $HEADER "\@protocol $forwardProtocolDeclaration;\n";
        }

        print $HEADER @headerContent;
        close($HEADER);
        undef($HEADER);
        
        @headerContentHeader = "";
        @headerContent = "";
        %headerForwardDeclarations = ();
    }
}

1;

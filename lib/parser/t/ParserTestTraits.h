/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __PARSERTESTTRAITS_H__
#define __PARSERTESTTRAITS_H__

#include <cxxtest/ValueTraits.h>

CXXTEST_ENUM_TRAITS (TestSemanticsHandler::TestType::Class,
		     CXXTEST_ENUM_MEMBER (TestSemanticsHandler::TestType::Base)
		     CXXTEST_ENUM_MEMBER (TestSemanticsHandler::TestType::Sampler)
		     CXXTEST_ENUM_MEMBER (TestSemanticsHandler::TestType::Array)
		     CXXTEST_ENUM_MEMBER (TestSemanticsHandler::TestType::Vector)
		     CXXTEST_ENUM_MEMBER (TestSemanticsHandler::TestType::Matrix)
);

CXXTEST_ENUM_TRAITS (s1::semantics::Name::NameType,
                     CXXTEST_ENUM_MEMBER (s1::semantics::Name::Variable)
                     CXXTEST_ENUM_MEMBER (s1::semantics::Name::Function)
                     CXXTEST_ENUM_MEMBER (s1::semantics::Name::TypeAlias)
);

#endif // __PARSERTESTTRAITS_H__

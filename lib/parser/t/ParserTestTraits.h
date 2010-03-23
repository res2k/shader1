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

#endif // __PARSERTESTTRAITS_H__

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

CXXTEST_ENUM_TRAITS (s1::parser::SemanticsHandler::Name::NameType,
		     CXXTEST_ENUM_MEMBER (s1::parser::SemanticsHandler::Name::Variable)
		     CXXTEST_ENUM_MEMBER (s1::parser::SemanticsHandler::Name::Function)
		     CXXTEST_ENUM_MEMBER (s1::parser::SemanticsHandler::Name::TypeAlias)
);

#endif // __PARSERTESTTRAITS_H__

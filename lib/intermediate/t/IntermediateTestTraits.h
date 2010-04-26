#ifndef __INTERMEDIATETESTTRAITS_H__
#define __INTERMEDIATETESTTRAITS_H__

#include <cxxtest/ValueTraits.h>

#include "TestSequenceVisitor.h"

CXXTEST_ENUM_TRAITS (TestSequenceVisitor::Operation,
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opConstBool)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opConstInt)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opConstUInt)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opConstFloat)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opAssignment)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCastToBool)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCastToInt)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCastToUInt)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCastToFloat)
);

#endif // __INTERMEDIATETESTTRAITS_H__

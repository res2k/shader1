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
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithAdd)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithSub)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithMul)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithDiv)
		     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithMod)
);

#endif // __INTERMEDIATETESTTRAITS_H__

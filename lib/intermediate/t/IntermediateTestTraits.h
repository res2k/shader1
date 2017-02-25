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
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeVectorBool)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeVectorInt)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeVectorUInt)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeVectorFloat)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeMatrixBool)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeMatrixInt)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeMatrixUInt)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeMatrixFloat)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opMakeArray)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opExtractArrayElement)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opExtractVectorComponent)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opChangeArrayElement)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opGetArrayLength)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithAdd)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithSub)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithMul)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithDiv)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opArithMod)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opLogicAnd)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opLogicOr)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opUnaryInv)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opUnaryNeg)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opUnaryNot)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCompareEq)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCompareNE)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCompareLE)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCompareLT)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCompareGE)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opCompareGT)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opBlock)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opBranch)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opWhile)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opReturn)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opFunctionCall)
                     CXXTEST_ENUM_MEMBER (TestSequenceVisitor::opBuiltinCall)
);

#endif // __INTERMEDIATETESTTRAITS_H__

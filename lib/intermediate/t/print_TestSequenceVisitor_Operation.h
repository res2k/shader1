/*
    Shader1
    Copyright (c) 2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Helper: Define operator<< for TestSequenceVisitor::Operation
 */

#ifndef PRINT_TESTSEQUENCEVISITOR_OPERATION_H_
#define PRINT_TESTSEQUENCEVISITOR_OPERATION_H_

#include "TestSequenceVisitor.h"

#include <iostream>

static std::ostream& operator<< (std::ostream& stream, const TestSequenceVisitor::Operation& op)
{
  switch (op)
  {
  #define PRINT_TOKEN(X)  case X: stream << #X; return stream;

  PRINT_TOKEN (TestSequenceVisitor::opConstBool)
  PRINT_TOKEN (TestSequenceVisitor::opConstInt)
  PRINT_TOKEN (TestSequenceVisitor::opConstUInt)
  PRINT_TOKEN (TestSequenceVisitor::opConstFloat)
  PRINT_TOKEN (TestSequenceVisitor::opAssignment)
  PRINT_TOKEN (TestSequenceVisitor::opCastToInvalid)
  PRINT_TOKEN (TestSequenceVisitor::opCastToBool)
  PRINT_TOKEN (TestSequenceVisitor::opCastToInt)
  PRINT_TOKEN (TestSequenceVisitor::opCastToUInt)
  PRINT_TOKEN (TestSequenceVisitor::opCastToFloat)
  PRINT_TOKEN (TestSequenceVisitor::opMakeVectorInvalid)
  PRINT_TOKEN (TestSequenceVisitor::opMakeVectorBool)
  PRINT_TOKEN (TestSequenceVisitor::opMakeVectorInt)
  PRINT_TOKEN (TestSequenceVisitor::opMakeVectorUInt)
  PRINT_TOKEN (TestSequenceVisitor::opMakeVectorFloat)
  PRINT_TOKEN (TestSequenceVisitor::opVectorCross)
  PRINT_TOKEN (TestSequenceVisitor::opVectorDot)
  PRINT_TOKEN (TestSequenceVisitor::opVectorLength)
  PRINT_TOKEN (TestSequenceVisitor::opVectorNormalize)
  PRINT_TOKEN (TestSequenceVisitor::opMakeMatrixInvalid)
  PRINT_TOKEN (TestSequenceVisitor::opMakeMatrixBool)
  PRINT_TOKEN (TestSequenceVisitor::opMakeMatrixInt)
  PRINT_TOKEN (TestSequenceVisitor::opMakeMatrixUInt)
  PRINT_TOKEN (TestSequenceVisitor::opMakeMatrixFloat)
  PRINT_TOKEN (TestSequenceVisitor::opMatrixLinAlgMul)
  PRINT_TOKEN (TestSequenceVisitor::opMakeArray)
  PRINT_TOKEN (TestSequenceVisitor::opExtractArrayElement)
  PRINT_TOKEN (TestSequenceVisitor::opExtractVectorComponent)
  PRINT_TOKEN (TestSequenceVisitor::opChangeArrayElement)
  PRINT_TOKEN (TestSequenceVisitor::opGetArrayLength)
  PRINT_TOKEN (TestSequenceVisitor::opArithAdd)
  PRINT_TOKEN (TestSequenceVisitor::opArithSub)
  PRINT_TOKEN (TestSequenceVisitor::opArithMul)
  PRINT_TOKEN (TestSequenceVisitor::opArithDiv)
  PRINT_TOKEN (TestSequenceVisitor::opArithMod)
  PRINT_TOKEN (TestSequenceVisitor::opLogicAnd)
  PRINT_TOKEN (TestSequenceVisitor::opLogicOr)
  PRINT_TOKEN (TestSequenceVisitor::opUnaryInv)
  PRINT_TOKEN (TestSequenceVisitor::opUnaryNeg)
  PRINT_TOKEN (TestSequenceVisitor::opUnaryNot)
  PRINT_TOKEN (TestSequenceVisitor::opCompareEq)
  PRINT_TOKEN (TestSequenceVisitor::opCompareNE)
  PRINT_TOKEN (TestSequenceVisitor::opCompareLE)
  PRINT_TOKEN (TestSequenceVisitor::opCompareLT)
  PRINT_TOKEN (TestSequenceVisitor::opCompareGE)
  PRINT_TOKEN (TestSequenceVisitor::opCompareGT)
  PRINT_TOKEN (TestSequenceVisitor::opBlock)
  PRINT_TOKEN (TestSequenceVisitor::opBranch)
  PRINT_TOKEN (TestSequenceVisitor::opWhile)
  PRINT_TOKEN (TestSequenceVisitor::opSampleTexture1D)
  PRINT_TOKEN (TestSequenceVisitor::opSampleTexture2D)
  PRINT_TOKEN (TestSequenceVisitor::opSampleTexture3D)
  PRINT_TOKEN (TestSequenceVisitor::opSampleTextureCUBE)
  PRINT_TOKEN (TestSequenceVisitor::opReturn)
  PRINT_TOKEN (TestSequenceVisitor::opFunctionCall)
  PRINT_TOKEN (TestSequenceVisitor::opBuiltinCall)

  #undef PRINT_TOKEN
  }
  stream << static_cast<int> (op);
  return stream;
}

#endif // PRINT_TESTSEQUENCEVISITOR_OPERATION_H_

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

#ifndef __INTERMEDIATE_NULLVISITOR_H__
#define __INTERMEDIATE_NULLVISITOR_H__

#include "SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    /**
     * SequenceVisitor implementation doing nothing.
     * Great base class for simple implementations.
     */
    struct NullVisitor : public SequenceVisitor
    {
      void PreVisitOp (const intermediate::SequenceOpPtr& op) override { }
      void PostVisitOp () override { }
      void VisitEnd () override { }

      void OpConstBool (const RegisterPtr& destination,
        bool value) override { }
      void OpConstInt (const RegisterPtr& destination,
        int value) override { }
      void OpConstUInt (const RegisterPtr& destination,
        unsigned int value) override { }
      void OpConstFloat (const RegisterPtr& destination,
        float value) override { }

      void OpAssign (const RegisterPtr& destination,
        const RegisterPtr& source) override { }

      void OpCast (const RegisterPtr& destination,
        BasicType destType,
        const RegisterPtr& source) override { }

      void OpMakeVector (const RegisterPtr& destination,
        BasicType compType,
        const std::vector<RegisterPtr>& sources) override { }

      void OpMakeMatrix (const RegisterPtr& destination,
        BasicType compType,
        unsigned int matrixRows, unsigned int matrixCols,
        const std::vector<RegisterPtr>& sources) override { }

      void OpMakeArray (const RegisterPtr& destination,
        const std::vector<RegisterPtr>& sources) override { }
      void OpExtractArrayElement (const RegisterPtr& destination,
        const RegisterPtr& source,
        const RegisterPtr& index) override { }
      void OpChangeArrayElement (const RegisterPtr& destination,
        const RegisterPtr& source,
        const RegisterPtr& index,
        const RegisterPtr& newValue) override { }
      void OpGetArrayLength (const RegisterPtr& destination,
        const RegisterPtr& array) override { }

      void OpExtractVectorComponent (const RegisterPtr& destination,
        const RegisterPtr& source,
        unsigned int comp) override { }

      void OpArith (const RegisterPtr& destination,
        ArithmeticOp op,
        const RegisterPtr& source1,
        const RegisterPtr& source2) override { }

      void OpCompare (const RegisterPtr& destination,
        CompareOp op,
        const RegisterPtr& source1,
        const RegisterPtr& source2) override { }

      void OpLogic (const RegisterPtr& destination,
        LogicOp op,
        const RegisterPtr& source1,
        const RegisterPtr& source2) override { }

      void OpUnary (const RegisterPtr& destination,
        UnaryOp op,
        const RegisterPtr& source) override { }

      void OpBlock (const SequencePtr& subSequence,
        const Sequence::IdentifierToRegMap& identToRegs_imp,
        const Sequence::IdentifierToRegMap& identToRegs_exp) override { }

      void OpBranch (const RegisterPtr& conditionReg,
        const SequenceOpPtr& seqOpIf,
        const SequenceOpPtr& seqOpElse) override { }
      void OpWhile (const RegisterPtr& conditionReg,
        const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
        const SequenceOpPtr& seqOpBody) override { }

      void OpReturn (const std::vector<RegisterPtr>& outParamVals) override { }
      void OpFunctionCall (const uc::String& funcIdent,
        const std::vector<RegisterPtr>& inParams,
        const std::vector<RegisterPtr>& outParams) override { }
      void OpBuiltinCall (const RegisterPtr& destination,
        BuiltinFunction what,
        const std::vector<RegisterPtr>& inParams) override { }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NULLVISITOR_H__

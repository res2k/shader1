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

#ifndef __INTERMEDIATE_SEQUENCEVISITOR_H__
#define __INTERMEDIATE_SEQUENCEVISITOR_H__

#include "forwarddecl.h"
#include "BasicType.h"
#include "BuiltinFunction.h"
#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor
    {
      virtual ~SequenceVisitor() {}
      
      virtual void SetVisitedOp (const intermediate::SequenceOpPtr& op) = 0;
      
      virtual void OpConstBool (const RegisterPtr& destination,
				bool value) = 0;
      virtual void OpConstInt (const RegisterPtr& destination,
			       int value) = 0;
      virtual void OpConstUInt (const RegisterPtr& destination,
				unsigned int value) = 0;
      virtual void OpConstFloat (const RegisterPtr& destination,
				 float value) = 0;
				 
      virtual void OpAssign (const RegisterPtr& destination,
			     const RegisterPtr& source) = 0;
				 
      virtual void OpCast (const RegisterPtr& destination,
			   BasicType destType,
			   const RegisterPtr& source) = 0;
				  
      virtual void OpMakeVector (const RegisterPtr& destination,
				 BasicType compType,
				 const std::vector<RegisterPtr>& sources) = 0;

      virtual void OpMakeMatrix (const RegisterPtr& destination,
				 BasicType compType,
				 unsigned int matrixRows, unsigned int matrixCols,
				 const std::vector<RegisterPtr>& sources) = 0;

      virtual void OpMakeArray (const RegisterPtr& destination,
				const std::vector<RegisterPtr>& sources) = 0;
      virtual void OpExtractArrayElement (const RegisterPtr& destination,
					  const RegisterPtr& source,
					  const RegisterPtr& index) = 0;
      virtual void OpChangeArrayElement (const RegisterPtr& destination,
					 const RegisterPtr& source,
					 const RegisterPtr& index,
					 const RegisterPtr& newValue) = 0;
      virtual void OpGetArrayLength (const RegisterPtr& destination,
				     const RegisterPtr& array) = 0;

      virtual void OpExtractVectorComponent (const RegisterPtr& destination,
					     const RegisterPtr& source,
					     unsigned int comp) = 0;
				 
      enum ArithmeticOp { Add, Sub, Mul, Div, Mod };
      virtual void OpArith (const RegisterPtr& destination,
			    ArithmeticOp op,
			    const RegisterPtr& source1,
			    const RegisterPtr& source2) = 0;

      enum CompareOp { Eq, NE, LT, LE, GT, GE };
      virtual void OpCompare (const RegisterPtr& destination,
			      CompareOp op,
			      const RegisterPtr& source1,
			      const RegisterPtr& source2) = 0;
				
      enum LogicOp { And, Or };
      virtual void OpLogic (const RegisterPtr& destination,
			    LogicOp op,
			    const RegisterPtr& source1,
			    const RegisterPtr& source2) = 0;

      enum UnaryOp { Neg, Inv, Not };
      virtual void OpUnary (const RegisterPtr& destination,
			    UnaryOp op,
			    const RegisterPtr& source) = 0;
			       
      virtual void OpBlock (const SequencePtr& subSequence,
			    const Sequence::IdentifierToRegMap& identToRegs_imp,
			    const Sequence::IdentifierToRegMap& identToRegs_exp) = 0;
			    
      virtual void OpBranch (const RegisterPtr& conditionReg,
			     const SequenceOpPtr& seqOpIf,
			     const SequenceOpPtr& seqOpElse) = 0;
      virtual void OpWhile (const RegisterPtr& conditionReg,
			    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
			    const SequenceOpPtr& seqOpBody) = 0;
			    
      virtual void OpReturn (const std::vector<RegisterPtr>& outParamVals) = 0;
      virtual void OpFunctionCall (const uc::String& funcIdent,
				   const std::vector<RegisterPtr>& inParams,
				   const std::vector<RegisterPtr>& outParams) = 0;
      virtual void OpBuiltinCall (const RegisterPtr& destination,
				  BuiltinFunction what,
				  const std::vector<RegisterPtr>& inParams) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEVISITOR_H__

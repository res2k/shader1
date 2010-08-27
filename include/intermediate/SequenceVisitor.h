#ifndef __INTERMEDIATE_SEQUENCEVISITOR_H__
#define __INTERMEDIATE_SEQUENCEVISITOR_H__

#include "forwarddecl.h"
#include "BuiltinFunction.h"
#include "RegisterID.h"
#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor
    {
      virtual ~SequenceVisitor() {}
      
      virtual void OpConstBool (const RegisterID& destination,
				bool value) = 0;
      virtual void OpConstInt (const RegisterID& destination,
			       int value) = 0;
      virtual void OpConstUInt (const RegisterID& destination,
				unsigned int value) = 0;
      virtual void OpConstFloat (const RegisterID& destination,
				 float value) = 0;
				 
      virtual void OpAssign (const RegisterID& destination,
			     const RegisterID& source) = 0;
				 
      enum BaseType { Bool, Int, UInt, Float };
      virtual void OpCast (const RegisterID& destination,
			   BaseType destType,
			   const RegisterID& source) = 0;
				  
      virtual void OpMakeVector (const RegisterID& destination,
				 BaseType compType,
				 const std::vector<RegisterID>& sources) = 0;

      virtual void OpMakeMatrix (const RegisterID& destination,
				 BaseType compType,
				 unsigned int matrixRows, unsigned int matrixCols,
				 const std::vector<RegisterID>& sources) = 0;

      virtual void OpMakeArray (const RegisterID& destination,
				const std::vector<RegisterID>& sources) = 0;
      virtual void OpExtractArrayElement (const RegisterID& destination,
					  const RegisterID& source,
					  const RegisterID& index) = 0;
      virtual void OpChangeArrayElement (const RegisterID& destination,
					 const RegisterID& source,
					 const RegisterID& index,
					 const RegisterID& newValue) = 0;
      virtual void OpGetArrayLength (const RegisterID& destination,
				     const RegisterID& array) = 0;

      virtual void OpExtractVectorComponent (const RegisterID& destination,
					     const RegisterID& source,
					     unsigned int comp) = 0;
				 
      enum ArithmeticOp { Add, Sub, Mul, Div, Mod };
      virtual void OpArith (const RegisterID& destination,
			    ArithmeticOp op,
			    const RegisterID& source1,
			    const RegisterID& source2) = 0;

      enum CompareOp { Eq, NE, LT, LE, GT, GE };
      virtual void OpCompare (const RegisterID& destination,
			      CompareOp op,
			      const RegisterID& source1,
			      const RegisterID& source2) = 0;
				
      enum LogicOp { And, Or };
      virtual void OpLogic (const RegisterID& destination,
			    LogicOp op,
			    const RegisterID& source1,
			    const RegisterID& source2) = 0;

      enum UnaryOp { Neg, Inv, Not };
      virtual void OpUnary (const RegisterID& destination,
			    UnaryOp op,
			    const RegisterID& source) = 0;
			       
      virtual void OpBlock (const SequencePtr& subSequence,
			    const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
			    const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
			    const std::vector<RegisterID>& writtenRegisters) = 0;
			    
      virtual void OpBranch (const RegisterID& conditionReg,
			     const SequenceOpPtr& seqOpIf,
			     const SequenceOpPtr& seqOpElse) = 0;
      virtual void OpWhile (const RegisterID& conditionReg,
			    const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
			    const SequenceOpPtr& seqOpBody) = 0;
			    
      virtual void OpReturn (const RegisterID& retValReg) = 0;
      virtual void OpFunctionCall (const RegisterID& destination,
				   const UnicodeString& funcIdent,
				   const std::vector<RegisterID>& inParams,
				   const std::vector<RegisterID>& outParams) = 0;
      virtual void OpBuiltinCall (const RegisterID& destination,
				  BuiltinFunction what,
				  const std::vector<RegisterID>& inParams) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEVISITOR_H__

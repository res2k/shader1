#ifndef __TESTSEQUENCEVISITOR_H__
#define __TESTSEQUENCEVISITOR_H__

#include "intermediate/Sequence.h"
#include "intermediate/SequenceVisitor.h"

class TestSequenceVisitor : public s1::intermediate::SequenceVisitor
{
public:
  enum Operation
  {
    opConstBool,
    opConstInt,
    opConstUInt,
    opConstFloat,
    opAssignment,
    opCastToBool,
    opCastToInt,
    opCastToUInt,
    opCastToFloat,
    opMakeVectorBool,
    opMakeVectorInt,
    opMakeVectorUInt,
    opMakeVectorFloat,
    opMakeMatrixBool,
    opMakeMatrixInt,
    opMakeMatrixUInt,
    opMakeMatrixFloat,
    opMakeArray,
    opExtractArrayElement,
    opExtractVectorComponent,
    opChangeArrayElement,
    opGetArrayLength,
    opArithAdd,
    opArithSub,
    opArithMul,
    opArithDiv,
    opArithMod,
    opLogicAnd,
    opLogicOr,
    opUnaryInv,
    opUnaryNeg,
    opUnaryNot,
    opCompareEq,
    opCompareNE,
    opCompareLE,
    opCompareLT,
    opCompareGE,
    opCompareGT,
    opReturn,
    opFunctionCall,
    opBuiltinCall
  };
  typedef s1::intermediate::RegisterPtr RegisterPtr;
  typedef s1::intermediate::Sequence Sequence;
  struct SequenceEntry
  {
    Operation op;
    
    union
    {
      bool boolConst;
      int intConst;
      unsigned int uintConst;
      float floatConst;
    };
      
    RegisterPtr destReg;
    RegisterPtr sourceReg[3];
    std::vector<RegisterPtr> sourceRegs;
    
    unsigned int matrixRows;
    unsigned int matrixCols;
    
    unsigned int extractComp;
    
    UnicodeString functionIdentifier;
    s1::intermediate::BuiltinFunction whatBuiltin;
    std::vector<RegisterPtr> inParams;
    std::vector<RegisterPtr> outParams;
  };
  
  std::vector<SequenceEntry> entries;
  
  void OpConstBool (const RegisterPtr& destination,
		    bool value)
  {
    SequenceEntry entry;
    entry.op = opConstBool;
    entry.destReg = destination;
    entry.boolConst = value;
    entries.push_back (entry);
  }
  
  void OpConstInt (const RegisterPtr& destination,
		   int value)
  {
    SequenceEntry entry;
    entry.op = opConstInt;
    entry.destReg = destination;
    entry.intConst = value;
    entries.push_back (entry);
  }
  
  void OpConstUInt (const RegisterPtr& destination,
		    unsigned int value)
  {
    SequenceEntry entry;
    entry.op = opConstUInt;
    entry.destReg = destination;
    entry.uintConst = value;
    entries.push_back (entry);
  }
  
  void OpConstFloat (const RegisterPtr& destination,
		     float value)
  {
    SequenceEntry entry;
    entry.op = opConstFloat;
    entry.destReg = destination;
    entry.floatConst = value;
    entries.push_back (entry);
  }
  
			      
  void OpAssign (const RegisterPtr& destination,
		 const RegisterPtr& source)
  {
    SequenceEntry entry;
    entry.op = opAssignment;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
			      
  void OpCast (const RegisterPtr& destination,
	       BaseType destType,
	       const RegisterPtr& source)
  {
    SequenceEntry entry;
    switch (destType)
    {
    case Bool:
      entry.op = opCastToBool;
      break;
    case Int:
      entry.op = opCastToInt;
      break;
    case UInt:
      entry.op = opCastToUInt;
      break;
    case Float:
      entry.op = opCastToFloat;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  
  void OpMakeVector (const RegisterPtr& destination,
		     BaseType compType,
		     const std::vector<RegisterPtr>& sources)
  {
    SequenceEntry entry;
    switch (compType)
    {
    case Bool:
      entry.op = opMakeVectorBool;
      break;
    case Int:
      entry.op = opMakeVectorInt;
      break;
    case UInt:
      entry.op = opMakeVectorUInt;
      break;
    case Float:
      entry.op = opMakeVectorFloat;
      break;
    }
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entries.push_back (entry);
  }


  void OpMakeMatrix (const RegisterPtr& destination,
		     BaseType compType,
		     unsigned int matrixRows, unsigned int matrixCols,
		     const std::vector<RegisterPtr>& sources)
  {
    SequenceEntry entry;
    switch (compType)
    {
    case Bool:
      entry.op = opMakeMatrixBool;
      break;
    case Int:
      entry.op = opMakeMatrixInt;
      break;
    case UInt:
      entry.op = opMakeMatrixUInt;
      break;
    case Float:
      entry.op = opMakeMatrixFloat;
      break;
    }
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entry.matrixRows = matrixRows;
    entry.matrixCols = matrixCols;
    entries.push_back (entry);
  }


  void OpMakeArray (const RegisterPtr& destination,
		    const std::vector<RegisterPtr>& sources)
  {
    SequenceEntry entry;
    entry.op = opMakeArray;
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entries.push_back (entry);
  }
				
  void OpExtractArrayElement (const RegisterPtr& destination,
			      const RegisterPtr& source,
			      const RegisterPtr& index)
  {
    SequenceEntry entry;
    entry.op = opExtractArrayElement;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.sourceReg[1] = index;
    entries.push_back (entry);
  }

  void OpChangeArrayElement (const RegisterPtr& destination,
			     const RegisterPtr& source,
			     const RegisterPtr& index,
			     const RegisterPtr& newValue)
  {
    SequenceEntry entry;
    entry.op = opChangeArrayElement;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.sourceReg[1] = index;
    entry.sourceReg[2] = newValue;
    entries.push_back (entry);
  }
      
  void OpGetArrayLength (const RegisterPtr& destination,
			 const RegisterPtr& array)
  {
    SequenceEntry entry;
    entry.op = opGetArrayLength;
    entry.destReg = destination;
    entry.sourceReg[0] = array;
    entries.push_back (entry);
  }

					 
  void OpExtractVectorComponent (const RegisterPtr& destination,
				 const RegisterPtr& source,
				 unsigned int comp)
  {
    SequenceEntry entry;
    entry.op = opExtractVectorComponent ;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.extractComp = comp;
    entries.push_back (entry);
  }
					     
					     
  void OpArith (const RegisterPtr& destination,
		ArithmeticOp op,
		const RegisterPtr& source1,
		const RegisterPtr& source2)
  {
    SequenceEntry entry;
    switch (op)
    {
    case Add:
      entry.op = opArithAdd;
      break;
    case Sub:
      entry.op = opArithSub;
      break;
    case Mul:
      entry.op = opArithMul;
      break;
    case Div:
      entry.op = opArithDiv;
      break;
    case Mod:
      entry.op = opArithMod;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }


  void OpLogic (const RegisterPtr& destination,
		LogicOp op,
		const RegisterPtr& source1,
		const RegisterPtr& source2)
  {
    SequenceEntry entry;
    switch (op)
    {
    case And:
      entry.op = opLogicAnd;
      break;
    case Or:
      entry.op = opLogicOr;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }


  void OpUnary (const RegisterPtr& destination,
		UnaryOp op,
		const RegisterPtr& source)
  {
    SequenceEntry entry;
    switch (op)
    {
    case Inv:
      entry.op = opUnaryInv;
      break;
    case Neg:
      entry.op = opUnaryNeg;
      break;
    case Not:
      entry.op = opUnaryNot;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }

  
  void OpCompare (const RegisterPtr& destination,
		  CompareOp op,
		  const RegisterPtr& source1,
		  const RegisterPtr& source2)
  {
    SequenceEntry entry;
    switch (op)
    {
    case Eq:
      entry.op = opCompareEq;
      break;
    case NE:
      entry.op = opCompareNE;
      break;
    case LT:
      entry.op = opCompareLT;
      break;
    case LE:
      entry.op = opCompareLE;
      break;
    case GT:
      entry.op = opCompareGT;
      break;
    case GE:
      entry.op = opCompareGE;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }
  
  void OpBlock (const boost::shared_ptr<Sequence>& seq,
		const Sequence::IdentifierToRegMap&,
		const Sequence::IdentifierToRegMap&,
		const std::vector<RegisterPtr>& writtenRegisters)
  {
  }
  
  void OpBranch (const RegisterPtr& conditionReg,
		 const s1::intermediate::SequenceOpPtr& seqOpIf,
		 const s1::intermediate::SequenceOpPtr& seqOpElse)
  {
  }
  
  void OpWhile (const RegisterPtr& conditionReg,
		const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
		const s1::intermediate::SequenceOpPtr& seqOpBody)
  {
  }

  void OpReturn (const std::vector<RegisterPtr>& outParamVals)
  {
    SequenceEntry entry;
    entry.op = opReturn;
    entry.sourceRegs = outParamVals;
    entries.push_back (entry);
  }
  
  void OpFunctionCall (const UnicodeString& funcIdent,
		       const std::vector<RegisterPtr>& inParams,
		       const std::vector<RegisterPtr>& outParams)
  {
    SequenceEntry entry;
    entry.op = opFunctionCall;
    entry.functionIdentifier = funcIdent;
    entry.inParams = inParams;
    entry.outParams = outParams;
    entries.push_back (entry);
  }
  
  void OpBuiltinCall (const RegisterPtr& destination,
		      s1::intermediate::BuiltinFunction what,
		      const std::vector<RegisterPtr>& inParams)
  {
    SequenceEntry entry;
    entry.op = opBuiltinCall;
    entry.destReg = destination;
    entry.whatBuiltin = what;
    entry.inParams = inParams;
    entries.push_back (entry);
  }
};

#endif // __TESTSEQUENCEVISITOR_H__

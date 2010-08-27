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
  typedef s1::intermediate::RegisterID RegisterID;
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
      
    RegisterID destReg;
    RegisterID sourceReg[3];
    std::vector<RegisterID> sourceRegs;
    
    unsigned int matrixRows;
    unsigned int matrixCols;
    
    unsigned int extractComp;
    
    UnicodeString functionIdentifier;
    s1::intermediate::BuiltinFunction whatBuiltin;
    std::vector<RegisterID> inParams;
    std::vector<RegisterID> outParams;
  };
  
  std::vector<SequenceEntry> entries;
  
  void OpConstBool (const RegisterID& destination,
		    bool value)
  {
    SequenceEntry entry;
    entry.op = opConstBool;
    entry.destReg = destination;
    entry.boolConst = value;
    entries.push_back (entry);
  }
  
  void OpConstInt (const RegisterID& destination,
		   int value)
  {
    SequenceEntry entry;
    entry.op = opConstInt;
    entry.destReg = destination;
    entry.intConst = value;
    entries.push_back (entry);
  }
  
  void OpConstUInt (const RegisterID& destination,
		    unsigned int value)
  {
    SequenceEntry entry;
    entry.op = opConstUInt;
    entry.destReg = destination;
    entry.uintConst = value;
    entries.push_back (entry);
  }
  
  void OpConstFloat (const RegisterID& destination,
		     float value)
  {
    SequenceEntry entry;
    entry.op = opConstFloat;
    entry.destReg = destination;
    entry.floatConst = value;
    entries.push_back (entry);
  }
  
			      
  void OpAssign (const RegisterID& destination,
		 const RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opAssignment;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
			      
  void OpCast (const RegisterID& destination,
	       BaseType destType,
	       const RegisterID& source)
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
  
  
  void OpMakeVector (const RegisterID& destination,
		     BaseType compType,
		     const std::vector<RegisterID>& sources)
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


  void OpMakeMatrix (const RegisterID& destination,
		     BaseType compType,
		     unsigned int matrixRows, unsigned int matrixCols,
		     const std::vector<RegisterID>& sources)
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


  void OpMakeArray (const RegisterID& destination,
		    const std::vector<RegisterID>& sources)
  {
    SequenceEntry entry;
    entry.op = opMakeArray;
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entries.push_back (entry);
  }
				
  void OpExtractArrayElement (const RegisterID& destination,
			      const RegisterID& source,
			      const RegisterID& index)
  {
    SequenceEntry entry;
    entry.op = opExtractArrayElement;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.sourceReg[1] = index;
    entries.push_back (entry);
  }

  void OpChangeArrayElement (const RegisterID& destination,
			     const RegisterID& source,
			     const RegisterID& index,
			     const RegisterID& newValue)
  {
    SequenceEntry entry;
    entry.op = opChangeArrayElement;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.sourceReg[1] = index;
    entry.sourceReg[2] = newValue;
    entries.push_back (entry);
  }
      
  void OpGetArrayLength (const RegisterID& destination,
			 const RegisterID& array)
  {
    SequenceEntry entry;
    entry.op = opGetArrayLength;
    entry.destReg = destination;
    entry.sourceReg[0] = array;
    entries.push_back (entry);
  }

					 
  void OpExtractVectorComponent (const RegisterID& destination,
				 const RegisterID& source,
				 unsigned int comp)
  {
    SequenceEntry entry;
    entry.op = opExtractVectorComponent ;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.extractComp = comp;
    entries.push_back (entry);
  }
					     
					     
  void OpArith (const RegisterID& destination,
		ArithmeticOp op,
		const RegisterID& source1,
		const RegisterID& source2)
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


  void OpLogic (const RegisterID& destination,
		LogicOp op,
		const RegisterID& source1,
		const RegisterID& source2)
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


  void OpUnary (const RegisterID& destination,
		UnaryOp op,
		const RegisterID& source)
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

  
  void OpCompare (const RegisterID& destination,
		  CompareOp op,
		  const RegisterID& source1,
		  const RegisterID& source2)
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
		const Sequence::IdentifierToRegIDMap&,
		const Sequence::IdentifierToRegIDMap&,
		const std::vector<RegisterID>& writtenRegisters)
  {
  }
  
  void OpBranch (const RegisterID& conditionReg,
		 const s1::intermediate::SequenceOpPtr& seqOpIf,
		 const s1::intermediate::SequenceOpPtr& seqOpElse)
  {
  }
  
  void OpWhile (const RegisterID& conditionReg,
		const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
		const s1::intermediate::SequenceOpPtr& seqOpBody)
  {
  }

  void OpReturn (const RegisterID& retValReg)
  {
    SequenceEntry entry;
    entry.op = opReturn;
    entry.sourceReg[0] = retValReg;
    entries.push_back (entry);
  }
  
  void OpFunctionCall (const RegisterID& destination,
		       const UnicodeString& funcIdent,
		       const std::vector<RegisterID>& inParams,
		       const std::vector<RegisterID>& outParams)
  {
    SequenceEntry entry;
    entry.op = opFunctionCall;
    entry.destReg = destination;
    entry.functionIdentifier = funcIdent;
    entry.inParams = inParams;
    entry.outParams = outParams;
    entries.push_back (entry);
  }
  
  void OpBuiltinCall (const RegisterID& destination,
		      s1::intermediate::BuiltinFunction what,
		      const std::vector<RegisterID>& inParams)
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

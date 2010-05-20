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
    opArithAdd,
    opArithSub,
    opArithMul,
    opArithDiv,
    opArithMod,
    opLogicAnd,
    opLogicOr,
    opCompareEq,
    opCompareNE,
    opCompareLE,
    opCompareLT,
    opCompareGE,
    opCompareGT,
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
    RegisterID sourceReg[2];
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
  
			      
  void OpCastToBool (const RegisterID& destination,
			      const RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToBool;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  void OpCastToInt (const RegisterID& destination,
			    const RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToInt;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  void OpCastToUInt (const RegisterID& destination,
			      const RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToUInt;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  void OpCastToFloat (const RegisterID& destination,
		      const RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToFloat;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  

  void OpArithAdd (const RegisterID& destination,
		   const RegisterID& source1,
		   const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithAdd;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithSub (const RegisterID& destination,
		   const RegisterID& source1,
		   const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithSub;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithMul (const RegisterID& destination,
		   const RegisterID& source1,
		   const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithMul;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithDiv (const RegisterID& destination,
		   const RegisterID& source1,
		   const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithDiv;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithMod (const RegisterID& destination,
		   const RegisterID& source1,
		   const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithMod;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }


  void OpLogicAnd (const RegisterID& destination,
		   const RegisterID& source1,
		   const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opLogicAnd;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpLogicOr (const RegisterID& destination,
		  const RegisterID& source1,
		  const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opLogicOr;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  
  void OpCompareEq (const RegisterID& destination,
		    const RegisterID& source1,
		    const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opCompareEq;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpCompareNE (const RegisterID& destination,
		    const RegisterID& source1,
		    const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opCompareNE;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpCompareLE (const RegisterID& destination,
		    const RegisterID& source1,
		    const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opCompareLE;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpCompareLT (const RegisterID& destination,
		    const RegisterID& source1,
		    const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opCompareLT;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpCompareGE (const RegisterID& destination,
		    const RegisterID& source1,
		    const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opCompareGE;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpCompareGT (const RegisterID& destination,
		    const RegisterID& source1,
		    const RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opCompareGT;
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
};

#endif // __TESTSEQUENCEVISITOR_H__

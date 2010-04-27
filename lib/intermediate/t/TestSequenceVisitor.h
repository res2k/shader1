#ifndef __TESTSEQUENCEVISITOR_H__
#define __TESTSEQUENCEVISITOR_H__

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
  };
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
      
    Sequence::RegisterID destReg;
    Sequence::RegisterID sourceReg[2];
  };
  
  std::vector<SequenceEntry> entries;
  
  void OpConstBool (const Sequence::RegisterID& destination,
		    bool value)
  {
    SequenceEntry entry;
    entry.op = opConstBool;
    entry.destReg = destination;
    entry.boolConst = value;
    entries.push_back (entry);
  }
  
  void OpConstInt (const Sequence::RegisterID& destination,
		   int value)
  {
    SequenceEntry entry;
    entry.op = opConstInt;
    entry.destReg = destination;
    entry.intConst = value;
    entries.push_back (entry);
  }
  
  void OpConstUInt (const Sequence::RegisterID& destination,
		    unsigned int value)
  {
    SequenceEntry entry;
    entry.op = opConstUInt;
    entry.destReg = destination;
    entry.uintConst = value;
    entries.push_back (entry);
  }
  
  void OpConstFloat (const Sequence::RegisterID& destination,
		     float value)
  {
    SequenceEntry entry;
    entry.op = opConstFloat;
    entry.destReg = destination;
    entry.floatConst = value;
    entries.push_back (entry);
  }
  
			      
  void OpAssign (const Sequence::RegisterID& destination,
		 const Sequence::RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opAssignment;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
			      
  void OpCastToBool (const Sequence::RegisterID& destination,
			      const Sequence::RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToBool;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  void OpCastToInt (const Sequence::RegisterID& destination,
			    const Sequence::RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToInt;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  void OpCastToUInt (const Sequence::RegisterID& destination,
			      const Sequence::RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToUInt;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  
  void OpCastToFloat (const Sequence::RegisterID& destination,
		      const Sequence::RegisterID& source)
  {
    SequenceEntry entry;
    entry.op = opCastToFloat;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  

  void OpArithAdd (const Sequence::RegisterID& destination,
		   const Sequence::RegisterID& source1,
		   const Sequence::RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithAdd;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithSub (const Sequence::RegisterID& destination,
		   const Sequence::RegisterID& source1,
		   const Sequence::RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithSub;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithMul (const Sequence::RegisterID& destination,
		   const Sequence::RegisterID& source1,
		   const Sequence::RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithMul;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithDiv (const Sequence::RegisterID& destination,
		   const Sequence::RegisterID& source1,
		   const Sequence::RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithDiv;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpArithMod (const Sequence::RegisterID& destination,
		   const Sequence::RegisterID& source1,
		   const Sequence::RegisterID& source2)
  {
    SequenceEntry entry;
    entry.op = opArithMod;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }
};

#endif // __TESTSEQUENCEVISITOR_H__

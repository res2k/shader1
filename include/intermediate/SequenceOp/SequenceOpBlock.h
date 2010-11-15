#ifndef __INTERMEDIATE_SEQUENCEOPBLOCK_H__
#define __INTERMEDIATE_SEQUENCEOPBLOCK_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"
#include "../Sequence.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBlock : public SequenceOp
    {
      SequencePtr subSequence;
      Sequence::IdentifierToRegMap identToRegs_imp;
      Sequence::IdentifierToRegMap identToRegs_exp;
      RegisterSet readRegisters;
      RegisterSet writtenRegisters;
    public:
      SequenceOpBlock (const SequencePtr& subSequence,
		       const Sequence::IdentifierToRegMap& identToRegs_imp,
		       const Sequence::IdentifierToRegMap& identToRegs_exp);
      
      RegisterSet GetReadRegisters () const;
      RegisterSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
      
      const SequencePtr& GetSequence() const { return subSequence; }
      const Sequence::IdentifierToRegMap& GetImportIdentToRegs() const
      { return identToRegs_imp; }
      const Sequence::IdentifierToRegMap& GetExportIdentToRegs() const
      { return identToRegs_exp; }
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBLOCK_H__

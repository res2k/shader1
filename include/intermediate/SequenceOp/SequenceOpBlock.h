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
      Sequence::IdentifierToRegIDMap identToRegIDs_imp;
      Sequence::IdentifierToRegIDMap identToRegIDs_exp;
      std::vector<RegisterID> readRegisters;
      std::vector<RegisterID> writtenRegisters;
    public:
      SequenceOpBlock (const SequencePtr& subSequence,
		       const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
		       const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
		       const std::vector<RegisterID>& readRegisters,
		       const std::vector<RegisterID>& writtenRegisters);
      
      RegisterIDSet GetReadRegisters () const;
      RegisterIDSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
      
      const SequencePtr& GetSequence() const { return subSequence; }
      const Sequence::IdentifierToRegIDMap& GetImportIdentToRegs() const
      { return identToRegIDs_imp; }
      const Sequence::IdentifierToRegIDMap& GetExportIdentToRegs() const
      { return identToRegIDs_exp; }
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBLOCK_H__

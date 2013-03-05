#ifndef __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__
#define __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"

#include <unicode/unistr.h>
#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpFunctionCall : public SequenceOp
    {
      UnicodeString funcIdent;
      std::vector<RegisterPtr> inParams;
      std::vector<RegisterPtr> outParams;
    public:
      SequenceOpFunctionCall (const UnicodeString& funcIdent,
			      const std::vector<RegisterPtr>& inParams,
			      const std::vector<RegisterPtr>& outParams);
      
      RegisterSet GetReadRegisters () const;
      RegisterSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__

#ifndef __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__
#define __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__

#include "SequenceOpWithResult.h"
#include "../forwarddecl.h"
#include "../RegisterID.h"

#include <unicode/unistr.h>
#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpFunctionCall : public SequenceOpWithResult
    {
      UnicodeString funcIdent;
      std::vector<RegisterID> inParams;
      std::vector<RegisterID> outParams;
    public:
      SequenceOpFunctionCall (const RegisterID& destination,
			      const UnicodeString& funcIdent,
			      const std::vector<RegisterID>& inParams,
			      const std::vector<RegisterID>& outParams);
      
      RegisterIDSet GetReadRegisters () const;
      RegisterIDSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__

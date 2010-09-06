#ifndef __INTERMEDIATE_PROGRAM_H__
#define __INTERMEDIATE_PROGRAM_H__

#include "forwarddecl.h"
#include "IntermediateGeneratorSemanticsHandler.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class Program
    {
    protected:
      friend class IntermediateGeneratorSemanticsHandler;
      
      std::vector<ProgramFunctionPtr> functions;
    public:
      void AddFunction (const ProgramFunctionPtr& func);
      
      size_t GetNumFunctions () const;
      ProgramFunctionPtr GetFunction (size_t n) const;
      
      void AddTransferValue (const IntermediateGeneratorSemanticsHandler::TypePtr& type,
			     const UnicodeString& name);
      typedef std::vector<std::pair<IntermediateGeneratorSemanticsHandler::TypePtr, UnicodeString> > TransferValues;
      const TransferValues& GetTransferValues () const { return transferValues; }
    private:
      TransferValues transferValues;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAM_H__
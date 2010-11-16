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
      typedef std::pair<IntermediateGeneratorSemanticsHandler::TypePtr, UnicodeString> TransferValuePair;
      typedef std::vector<TransferValuePair> TransferValues;
      const TransferValues& GetTransferValues () const { return transferValues; }
      
      void SetVertexOutputParameter (const UnicodeString& name) { vertexOut = name; }
      const UnicodeString& GetVertexOutputParameter () const { return vertexOut; }
      void SetFragmentOutputParameter (const UnicodeString& name) { fragmentOut = name; }
      const UnicodeString& GetFragmentOutputParameter () const { return fragmentOut; }
    private:
      TransferValues transferValues;
      UnicodeString vertexOut;
      UnicodeString fragmentOut;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAM_H__
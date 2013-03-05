#ifndef __INTERMEDIATE_PROGRAM_H__
#define __INTERMEDIATE_PROGRAM_H__

#include "forwarddecl.h"
#include "IntermediateGeneratorSemanticsHandler.h"

#include <boost/unordered_map.hpp>
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
      
      enum ParameterTarget
      {
	/// Vertex output position
	Position,
	/// Fragment output color
	Color
      };
      typedef boost::unordered_map<UnicodeString, ParameterTarget> OutputParameters;
      void SetOutputParameter (const UnicodeString& name, ParameterTarget target);
      const OutputParameters& GetOutputParameters () const;
      
      typedef boost::unordered_map<UnicodeString, size_t> ParameterArraySizes;
      void SetParameterArraySize (const UnicodeString& name, size_t size);
      const ParameterArraySizes& GetParameterArraySizes () const;
      
      // @@@ Actualls, should perhaps better be in splitter ...
      static int GetTargetFrequency (ParameterTarget target);
    private:
      TransferValues transferValues;
      OutputParameters outputParams;
      ParameterArraySizes paramArraySizes;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAM_H__
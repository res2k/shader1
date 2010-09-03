#ifndef __SPLITTER_PROGRAMSPLITTER_H__
#define __SPLITTER_PROGRAMSPLITTER_H__

#include "base/unordered_map"
#include "intermediate/forwarddecl.h"
#include "parser/SemanticsHandler.h"

#include "Frequency.h"

#include <unicode/unistr.h>

namespace s1
{
  namespace splitter
  {
    class SequenceSplitter;
    
    class ProgramSplitter
    {
      intermediate::ProgramPtr inputProgram;
      intermediate::ProgramPtr outputProgram;
      
      typedef std::tr1::unordered_map<UnicodeString, unsigned int> ParamMap;
      ParamMap paramFlags;
      
      friend class SequenceSplitter;
      struct FunctionTransferValues
      {
	parser::SemanticsHandler::TypePtr valueType;
	// Probably sensible: std::vector<size_t> usedArgs;
      };
      struct SplitFunctionInfo
      {
	UnicodeString funcVName;
	UnicodeString funcFName;
	std::vector<unsigned int> outputParamFreqs;
	std::vector<FunctionTransferValues> transferValues[freqNum-1];
      };
      typedef boost::shared_ptr<SplitFunctionInfo> SplitFunctionInfoPtr;
      typedef std::tr1::unordered_map<UnicodeString, SplitFunctionInfoPtr> SplitFunctionInfoMap;
      SplitFunctionInfoMap splitFunctions;
      /**
       * \param originalIdent Original identifier of function.
       * \param inputParamFreqs Frequency flags of function input parameters.
       * \param freqFuncIdents Output: Identifiers of function variants for the different frequencies.
       * \param returnFreq Output: Receives frequency flags of return value.
       * \param outputParamFreqs Output: Receives frequency flags of output parameters.
       * \param transferFlags Output: Values function needs transferred between frequencies.
       *   Each transferred value corresponds to an extra output parameter in the source frequency
       *   and an extra input parameter in the destination frequency.
       */
      void GetSplitFunctions (const UnicodeString& originalIdent,
			      const std::vector<unsigned int>& inputParamFreqFlags,
			      UnicodeString freqFuncIdents[freqNum],
			      unsigned int& returnFreq,
			      std::vector<unsigned int>& outputParamFreqs,
			      std::vector<FunctionTransferValues> transferValues[freqNum-1]);
			      
      intermediate::ProgramFunctionPtr FindProgramFunction (const UnicodeString& ident);
      
      void AddFreqFunction (const UnicodeString& funcName,
			    const intermediate::ProgramFunctionPtr& originalFunc,
			    const parser::SemanticsHandler::Scope::FunctionFormalParameters& extraParams,
			    const intermediate::SequencePtr& sequence);
			    
      class RecursionChecker;
      bool CheckFuncRecursive (const intermediate::ProgramFunctionPtr& func);
    public:
      void SetInputProgram (const intermediate::ProgramPtr& program);
      void SetInputFreqFlags (const UnicodeString& inpName, unsigned int flags);
      
      void PerformSplit ();
      
      const intermediate::ProgramPtr& GetOutputProgram ();
    };
  } // namespace splitter
} // namespace s1
    
#endif // __SPLITTER_PROGRAMSPLITTER_H__

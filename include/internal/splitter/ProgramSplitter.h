/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __SPLITTER_PROGRAMSPLITTER_H__
#define __SPLITTER_PROGRAMSPLITTER_H__

#include "intermediate/forwarddecl.h"
#include "semantics/Handler.h"
#include "semantics/Scope.h"

#include "Frequency.h"

#include <unordered_map>

namespace s1
{
  namespace splitter
  {
    class SequenceSplitter;
    
    class ProgramSplitter
    {
      intermediate::ProgramPtr inputProgram;
      intermediate::ProgramPtr outputPrograms[freqNum];
      
      typedef std::unordered_map<uc::String, unsigned int> ParamMap;
      ParamMap paramFlags;
      
      friend class SequenceSplitter;
      struct FunctionTransferValues
      {
        uc::String identifier;
        semantics::TypePtr valueType;
        // Probably sensible: std::vector<size_t> usedArgs;
      };
      struct SplitFunctionInfo
      {
        uc::String funcName[freqNum];
        std::vector<unsigned int> outputParamFreqs;
        std::vector<FunctionTransferValues> transferValues[freqNum-1];
      };
      typedef boost::shared_ptr<SplitFunctionInfo> SplitFunctionInfoPtr;
      typedef std::unordered_map<uc::String, SplitFunctionInfoPtr> SplitFunctionInfoMap;
      SplitFunctionInfoMap splitFunctions;

      struct SplitFunctionsResult
      {
        /// Identifiers of function variants for the different frequencies
        uc::String idents[freqNum];
        /// Frequency flags of return values.
        std::vector<unsigned int> outputParamFreqs;
        /**
         * Values function needs transferred between frequencies.
         * Each transferred value corresponds to an extra output parameter in the source frequency
         * and an extra input parameter in the destination frequency.
         */
        std::vector<FunctionTransferValues> transferValues[freqNum - 1];
        /// Force propagation of all inputs to split function frequency
        bool forceInputPropagation = false;
      };
      /**
       * \param originalIdent Original identifier of function.
       * \param inputParamFreqs Frequency flags of function input parameters.
       * \param outputParamFreqs Output: Receives frequency flags of output parameters.
       */
      SplitFunctionsResult GetSplitFunctions (const uc::String& originalIdent,
                                              const std::vector<unsigned int>& inputParamFreqFlags);

      intermediate::ProgramFunctionPtr FindProgramFunction (const uc::String& ident);
      
      void AddFreqFunction (const uc::String& funcName,
                            const intermediate::ProgramFunctionPtr& originalFunc,
                            const semantics::Scope::FunctionFormalParameters& extraParams,
                            const intermediate::SequencePtr& sequence,
                            int freq);
                            
      bool CheckFuncRecursive (const intermediate::ProgramFunctionPtr& func);
    public:
      ProgramSplitter ();

      void SetInputProgram (const intermediate::ProgramPtr& program);
      void SetInputFreqFlags (const uc::String& inpName, unsigned int flags);

      void PerformSplit ();

      const intermediate::ProgramPtr& GetOutputProgram (int frequency);
    };
  } // namespace splitter
} // namespace s1
    
#endif // __SPLITTER_PROGRAMSPLITTER_H__

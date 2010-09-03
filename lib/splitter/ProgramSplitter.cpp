#include "base/common.h"
#include "base/hash_UnicodeString.h"

#include "splitter/ProgramSplitter.h"

#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "splitter/SequenceSplitter.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <unicode/ustdio.h>

namespace s1
{
  namespace splitter
  {
    static std::string FreqFlagsSignature (const std::vector<unsigned int>& inputParamFreqFlags)
    {
      std::string sig;
      BOOST_FOREACH (unsigned int freqFlag, inputParamFreqFlags)
      {
	char ch = 0;
	for (int f = 0; f < freqNum; f++)
	{
	  if ((freqFlag & (1 << f)) != 0)
	  {
	    ch = '0' + f;
	    break;
	  }
	}
	assert (ch != 0);
	sig.push_back (ch);
      }
      return sig;
    }
    
    void ProgramSplitter::GetSplitFunctions (const UnicodeString& originalIdent,
					     const std::vector<unsigned int>& inputParamFreqFlags,
					     UnicodeString freqFuncIdents[freqNum],
					     unsigned int& returnFreq,
					     std::vector<unsigned int>& outputParamFreqs,
					     std::vector<FunctionTransferValues> transferValues[freqNum-1])
    {
      std::string freqSig (FreqFlagsSignature (inputParamFreqFlags));
      
      UnicodeString decoratedIdent (originalIdent);
      decoratedIdent.append ("$");
      decoratedIdent.append (freqSig.c_str());
      
      SplitFunctionInfoMap::const_iterator splitFuncInfo = splitFunctions.find (decoratedIdent);
      if (splitFuncInfo != splitFunctions.end())
      {
      #if 0
	if (splitFuncInfo->second->beingSetup)
	{
	  /*
	    Recursion:
	    When coming upon an input frequency combination that is currently split,
	    guess the frequencies of output parameters by choosing "all" frequencies...
	  */
	}
	else
	{
	}
      #endif
      }
      else
      {
	bool isRecursive = false; // @@@ TODO: Check for recursion
	if (isRecursive)
	{
	  /* Recursion is tricky.
	     For now, just force recursive functions to fragment frequency.
	     (This will be suboptimal in certain cases, but it is comparatively
	     easy to implement ...)
	     @@@ Force SequenceSplitter to promote all inputs to fragment!
	     -> Could also let SequenceSplitter deal with transfers by having 
	        promote the 'transfer' args
	     -> Effectively, could give every variant of a func a distinctive
	        arguments list...?
	   */
	}
	else
	{
	  SplitFunctionInfoPtr newFunc (boost::make_shared<SplitFunctionInfo> ());
	  splitFunctions[decoratedIdent] = newFunc;
	  
	  intermediate::ProgramFunctionPtr progFunc = FindProgramFunction (originalIdent);
	  
	  SequenceSplitter seqSplit (*this);
	  seqSplit.SetInputSequence (progFunc->GetBody());
	  
	  // Connect the parameter frequencies to actual inputs
	  const parser::SemanticsHandler::Scope::FunctionFormalParameters& funcParams = progFunc->GetParams();
	  for (size_t i = 0; i < inputParamFreqFlags.size(); i++)
	  {
	    seqSplit.SetInputFreqFlags (funcParams[i].identifier, inputParamFreqFlags[i]);
	  }
	  
	  seqSplit.PerformSplit();
	  
	  // Turn values 'transferred' by the function into extra output/input paramerts
	  parser::SemanticsHandler::Scope::FunctionFormalParameters extraParamsV;
	  parser::SemanticsHandler::Scope::FunctionFormalParameters extraParamsF;
	  const std::vector<intermediate::RegisterID>& transfers = seqSplit.GetTransferRegs (freqVertex);
	  unsigned int n = 0;
	  BOOST_FOREACH (const intermediate::RegisterID& reg, transfers)
	  {
	    // Get register bank, for type
	    intermediate::Sequence::RegisterBankPtr regBank;
	    seqSplit.GetOutputVertexSequence()->QueryRegisterPtrFromID (reg, regBank);
	    
	    // Generate unique parameter identifier
	    UnicodeString transferIdent ("transferP$");
	    UChar transferSuffix[charsToFormatUint + 1];
	    u_snprintf (transferSuffix, sizeof (transferSuffix)/sizeof (UChar),
			"%u", n++);
	    transferIdent.append (transferSuffix);
	    
	    // Synthesize new parameters
	    {
	      parser::SemanticsHandler::Scope::FunctionFormalParameter paramV;
	      paramV.dir = parser::SemanticsHandler::Scope::dirOut;
	      paramV.identifier = transferIdent;
	      paramV.type = regBank->GetOriginalType();
	      extraParamsV.push_back (paramV);
	      
	      seqSplit.GetOutputVertexSequence()->SetExport (transferIdent, reg);
	    }
	    {
	      parser::SemanticsHandler::Scope::FunctionFormalParameter paramF;
	      paramF.dir = parser::SemanticsHandler::Scope::dirIn;
	      paramF.identifier = transferIdent;
	      paramF.type = regBank->GetOriginalType();
	      extraParamsF.push_back (paramF);
	      
	      seqSplit.GetOutputFragmentSequence()->AddImport (transferIdent, reg);
	    }
	    
	    // Also return transfer info
	    FunctionTransferValues newTransferValue;
	    newTransferValue.valueType = regBank->GetOriginalType();
	    transferValues[freqVertex].push_back (newTransferValue);
	  }
	  
	  // Extract output parameters, to return output value frequencies
	  const intermediate::Sequence::RegisterExpMappings& seqExports = progFunc->GetBody()->GetExports();
	  BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& funcParam, funcParams)
	  {
	    if ((int (funcParam.dir) & parser::SemanticsHandler::Scope::dirOut) == 0) continue;
	    
	    intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (funcParam.identifier);
	    assert (exp != seqExports.end());
	    
	    intermediate::RegisterID seqRegID (exp->second);
	    outputParamFreqs.push_back (seqSplit.GetLocalRegFreqFlags (seqRegID));
	  }
      
	  // Generate 'split' functions
	  if ((seqSplit.GetOutputVertexSequence()->GetNumOps() > 0) || (extraParamsV.size() > 0))
	  {
	    UnicodeString funcVName ("vertex_");
	    funcVName.append (decoratedIdent);
	    AddFreqFunction (funcVName, progFunc, extraParamsV, seqSplit.GetOutputVertexSequence());
	    freqFuncIdents[freqVertex] = funcVName;
	    newFunc->funcVName = funcVName;
	  }
	  
	  if ((seqSplit.GetOutputFragmentSequence()->GetNumOps() > 0) || (extraParamsF.size() > 0))
	  {
	    UnicodeString funcFName ("fragment_");
	    funcFName.append (decoratedIdent);
	    AddFreqFunction (funcFName, progFunc, extraParamsF, seqSplit.GetOutputFragmentSequence());
	    freqFuncIdents[freqFragment] = funcFName;
	    newFunc->funcFName = funcFName;
	  }
	}
      }
    }
    
    intermediate::ProgramFunctionPtr ProgramSplitter::FindProgramFunction (const UnicodeString& ident)
    {
      for (size_t i = 0; i < inputProgram->GetNumFunctions(); i++)
      {
	intermediate::ProgramFunctionPtr func = inputProgram->GetFunction (i);
	if (func->GetIdentifier() == ident) return func;
      }
      return intermediate::ProgramFunctionPtr ();
    }
    
    void ProgramSplitter::AddFreqFunction (const UnicodeString& funcName,
					   const intermediate::ProgramFunctionPtr& originalFunc,
					   const parser::SemanticsHandler::Scope::FunctionFormalParameters& extraParams,
					   const intermediate::SequencePtr& sequence)
    {
      parser::SemanticsHandler::Scope::FunctionFormalParameters params (originalFunc->GetParams());
      params.insert (params.end(), extraParams.begin(), extraParams.end());
      
      intermediate::ProgramFunctionPtr newFunc (boost::make_shared<intermediate::ProgramFunction> (funcName,
												   originalFunc->GetReturnType(),
												   params,
												   sequence,
												   false));
      outputProgram->AddFunction (newFunc);
    }
    
    void ProgramSplitter::SetInputProgram (const intermediate::ProgramPtr& program)
    {
      inputProgram = program;
    }

    void ProgramSplitter::SetInputFreqFlags (const UnicodeString& inpName, unsigned int flags)
    {
      paramFlags[inpName] = flags;
    }
    
    void ProgramSplitter::PerformSplit ()
    {
      outputProgram = boost::make_shared<intermediate::Program> ();
  
      for (size_t i = 0; i < inputProgram->GetNumFunctions(); i++)
      {
	intermediate::ProgramFunctionPtr func = inputProgram->GetFunction (i);
	if (!func->IsEntryFunction()) continue;
	
	splitter::SequenceSplitter splitter (*this);
	splitter.SetInputSequence (func->GetBody());
	for (ParamMap::const_iterator paramFlag = paramFlags.begin();
	     paramFlag != paramFlags.end();
	     ++paramFlag)
	{
	  splitter.SetInputFreqFlags (paramFlag->first, paramFlag->second);
	}
	splitter.PerformSplit();
	
	UnicodeString funcVName ("vertex_");
	funcVName.append (func->GetIdentifier());
	
	intermediate::ProgramFunctionPtr funcV (boost::make_shared<intermediate::ProgramFunction> (funcVName,
												   func->GetReturnType(),
												   func->GetParams(),
												   splitter.GetOutputVertexSequence(),
												   func->IsEntryFunction()));
	outputProgram->AddFunction (funcV);

	UnicodeString funcFName ("fragment_");
	funcFName.append (func->GetIdentifier());
	
	intermediate::ProgramFunctionPtr funcF (boost::make_shared<intermediate::ProgramFunction> (funcFName,
												   func->GetReturnType(),
												   func->GetParams(),
												   splitter.GetOutputFragmentSequence(),
												   func->IsEntryFunction()));
	outputProgram->AddFunction (funcF);
      }
    }
    
    const intermediate::ProgramPtr& ProgramSplitter::GetOutputProgram ()
    {
      return outputProgram;
    }
    
  } // namespace splitter
} // namespace s1

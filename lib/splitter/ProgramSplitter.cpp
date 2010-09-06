#include "base/common.h"
#include "base/hash_UnicodeString.h"
#include "base/unordered_set"

#include "splitter/ProgramSplitter.h"

#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
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
	SplitFunctionInfoPtr funcInfo (splitFuncInfo->second);
	freqFuncIdents[freqVertex] = funcInfo->funcVName;
	freqFuncIdents[freqFragment] = funcInfo->funcFName;
	outputParamFreqs = funcInfo->outputParamFreqs;
	for (int f = 0; f < freqNum-1; f++)
	  transferValues[f] = funcInfo->transferValues[f];
      }
      else
      {
	SplitFunctionInfoPtr newFunc (boost::make_shared<SplitFunctionInfo> ());
	splitFunctions[decoratedIdent] = newFunc;
	
	intermediate::ProgramFunctionPtr progFunc = FindProgramFunction (originalIdent);
	  
	SequenceSplitter seqSplit (*this);
	seqSplit.SetInputSequence (progFunc->GetBody());
	
	bool isRecursive = CheckFuncRecursive (progFunc);
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
	  
	  /* Fake output parameter frequencies to 'fragment'
	     (so the recursive call will have something) */
	  const parser::SemanticsHandler::Scope::FunctionFormalParameters& funcParams = progFunc->GetParams();
	  const intermediate::Sequence::RegisterExpMappings& seqExports = progFunc->GetBody()->GetExports();
	  BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& funcParam, funcParams)
	  {
	    if ((int (funcParam.dir) & parser::SemanticsHandler::Scope::dirOut) == 0) continue;
	    
	    intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (funcParam.identifier);
	    assert (exp != seqExports.end());
	    
	    intermediate::RegisterID seqRegID (exp->second);
	    //outputParamFreqs.push_back (seqSplit.GetLocalRegFreqFlags (seqRegID));
	    outputParamFreqs.push_back (freqFlagF);
	  }
	  newFunc->outputParamFreqs = outputParamFreqs;
      
	  // Generate 'split' functions
	  UnicodeString funcFName ("fragment_");
	  funcFName.append (decoratedIdent);
	  freqFuncIdents[freqFragment] = funcFName;
	  newFunc->funcFName = funcFName;
	  
	  // Fake all inputs to fragment frequency
	  for (size_t i = 0; i < inputParamFreqFlags.size(); i++)
	  {
	    seqSplit.SetInputFreqFlags (funcParams[i].identifier,
					(inputParamFreqFlags[i] & freqFlagU) | freqFlagF);
	  }

	  seqSplit.PerformSplit();
	  
	  // Generate 'split' functions
	  parser::SemanticsHandler::Scope::FunctionFormalParameters extraParamsF;
	  AddFreqFunction (funcFName, progFunc, extraParamsF, seqSplit.GetOutputFragmentSequence());
	}
	else
	{
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
	  newFunc->transferValues[freqVertex] = transferValues[freqVertex];
	  
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
	  newFunc->outputParamFreqs = outputParamFreqs;
      
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
    
    class ProgramSplitter::RecursionChecker : public intermediate::SequenceVisitor
    {
      ProgramSplitter& parent;
      UnicodeString funcToCheck;
      bool recursionFound;
      
      std::tr1::unordered_set<UnicodeString> seenFunctions;
    public:
      RecursionChecker (ProgramSplitter& parent, const UnicodeString& funcToCheck)
       : parent (parent), funcToCheck (funcToCheck), recursionFound (false) {}
      
      bool WasRecursionFound() const { return recursionFound; }
      
      typedef intermediate::RegisterID RegisterID;
      void OpConstBool (const RegisterID&, bool) {}
      void OpConstInt (const RegisterID&, int) {}
      void OpConstUInt (const RegisterID&, unsigned int) {}
      void OpConstFloat (const RegisterID&, float) {}
				
      void OpAssign (const RegisterID&, const RegisterID&) {}
				
      void OpCast (const RegisterID&, BaseType, const RegisterID&) {}
				  
      void OpMakeVector (const RegisterID&, BaseType, const std::vector<RegisterID>&) {}

      void OpMakeMatrix (const RegisterID&, BaseType, unsigned int, unsigned int,
			 const std::vector<RegisterID>&) {}

      void OpMakeArray (const RegisterID&, const std::vector<RegisterID>&) {}
      void OpExtractArrayElement (const RegisterID&, const RegisterID&, const RegisterID&) {}
      void OpChangeArrayElement (const RegisterID&, const RegisterID&, const RegisterID&, const RegisterID&) {}
      void OpGetArrayLength (const RegisterID&, const RegisterID&) {}

      void OpExtractVectorComponent (const RegisterID&, const RegisterID&, unsigned int) {}
				
      void OpArith (const RegisterID&, ArithmeticOp, const RegisterID&, const RegisterID&) {}
      void OpCompare (const RegisterID&, CompareOp, const RegisterID&, const RegisterID&) {}
      void OpLogic (const RegisterID&, LogicOp, const RegisterID&, const RegisterID&) {}
      void OpUnary (const RegisterID&, UnaryOp, const RegisterID&) {}
			      
      void OpBlock (const intermediate::SequencePtr& subSequence,
		    const intermediate::Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
		    const intermediate::Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
		    const std::vector<RegisterID>& writtenRegisters)
      {
	if (recursionFound) return;
	subSequence->Visit (*this);
      }
			    
      void OpBranch (const RegisterID& conditionReg,
		      const intermediate::SequenceOpPtr& seqOpIf,
		      const intermediate::SequenceOpPtr& seqOpElse)
      {
	if (recursionFound) return;
	boost::shared_ptr<intermediate::SequenceOpBlock> ifBlock (
	  boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpIf));
	assert (ifBlock);
	ifBlock->GetSequence()->Visit (*this);
	
	if (recursionFound) return;
	boost::shared_ptr<intermediate::SequenceOpBlock> elseBlock (
	  boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpElse));
	assert (elseBlock);
	elseBlock->GetSequence()->Visit (*this);
      }
      
      void OpWhile (const RegisterID& conditionReg,
		    const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
		    const intermediate::SequenceOpPtr& seqOpBody)
      {
	if (recursionFound) return;
	boost::shared_ptr<intermediate::SequenceOpBlock> block (
	  boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpBody));
	assert (block);
	block->GetSequence()->Visit (*this);
      }
			    
      void OpReturn (const RegisterID&) {}
      void OpFunctionCall (const RegisterID& destination,
			   const UnicodeString& funcIdent,
			   const std::vector<RegisterID>& inParams,
			   const std::vector<RegisterID>& outParams)
      {
	if (recursionFound) return;
	
	if (funcIdent == funcToCheck)
	{
	  recursionFound = true;
	  return;
	}
	
	if (seenFunctions.find (funcIdent) == seenFunctions.end())
	{
	  seenFunctions.insert (funcIdent);
	  
	  intermediate::ProgramFunctionPtr func = parent.FindProgramFunction (funcIdent);
	  func->GetBody()->Visit (*this);
	}
      }
      
      void OpBuiltinCall (const RegisterID&, intermediate::BuiltinFunction, const std::vector<RegisterID>&) {}
    };
    
    bool ProgramSplitter::CheckFuncRecursive (const intermediate::ProgramFunctionPtr& func)
    {
      RecursionChecker recCheck (*this, func->GetIdentifier());
      func->GetBody()->Visit (recCheck);
      return recCheck.WasRecursionFound();
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
	
	parser::SemanticsHandler::Scope::FunctionFormalParameters vParams;
	parser::SemanticsHandler::Scope::FunctionFormalParameters fParams;
	BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& param, func->GetParams())
	{
	  int paramFreq;
	  ParamMap::const_iterator paramFlag = paramFlags.find (param.identifier);
	  if (paramFlag != paramFlags.end())
	  {
	    paramFreq = paramFlag->second;
	  }
	  else
	  {
	    paramFreq = SequenceSplitter::GetDefaultFrequencyForType (param.type);
	  }
	  splitter.SetInputFreqFlags (param.identifier, paramFreq);
	  if ((paramFreq & (freqFlagU | freqFlagV)) != 0)
	    vParams.push_back (param);
	  if ((paramFreq & (freqFlagU | freqFlagF)) != 0)
	    fParams.push_back (param);
	}
	
	splitter.PerformSplit();
	
	UnicodeString funcVName ("vertex_");
	funcVName.append (func->GetIdentifier());
	
	intermediate::ProgramFunctionPtr funcV (boost::make_shared<intermediate::ProgramFunction> (funcVName,
												   func->GetReturnType(),
												   vParams,
												   splitter.GetOutputVertexSequence(),
												   func->IsEntryFunction()));
	funcV->SetExecutionFrequence (freqVertex);
	outputProgram->AddFunction (funcV);

	UnicodeString funcFName ("fragment_");
	funcFName.append (func->GetIdentifier());
	
	intermediate::ProgramFunctionPtr funcF (boost::make_shared<intermediate::ProgramFunction> (funcFName,
												   func->GetReturnType(),
												   fParams,
												   splitter.GetOutputFragmentSequence(),
												   func->IsEntryFunction()));
	funcF->SetExecutionFrequence (freqFragment);
	outputProgram->AddFunction (funcF);
	
	const std::vector<intermediate::RegisterID>& transverV2F = splitter.GetTransferRegs (freqVertex);
	BOOST_FOREACH(const intermediate::RegisterID& reg, transverV2F)
	{
	  intermediate::Sequence::RegisterBankPtr regBank;
	  intermediate::Sequence::RegisterPtr regPtr (
	    splitter.GetOutputVertexSequence()->QueryRegisterPtrFromID (reg, regBank));
	  
	  outputProgram->AddTransferValue (regBank->GetOriginalType(), regPtr->GetName());
	  funcV->SetTransferMapping (regPtr->GetName(), reg);
	  funcF->SetTransferMapping (regPtr->GetName(), reg);
	}
      }
    }
    
    const intermediate::ProgramPtr& ProgramSplitter::GetOutputProgram ()
    {
      return outputProgram;
    }
    
  } // namespace splitter
} // namespace s1

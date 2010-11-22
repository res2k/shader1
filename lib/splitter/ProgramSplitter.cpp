#include "base/common.h"

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
	for (int f = 0; f < freqNum; f++)
	  freqFuncIdents[f] = funcInfo->funcName[f];
	outputParamFreqs = funcInfo->outputParamFreqs;
	for (int f = 0; f < freqNum-1; f++)
	  transferValues[f] = funcInfo->transferValues[f];
      }
      else
      {
	SplitFunctionInfoPtr newFunc (boost::make_shared<SplitFunctionInfo> ());
	splitFunctions[decoratedIdent] = newFunc;
	
	intermediate::ProgramFunctionPtr progFunc = FindProgramFunction (originalIdent);
	  
	SequenceSplitter seqSplit (*this, false);
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
	    
	    #if 0
	    intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (funcParam.identifier);
	    assert (exp != seqExports.end());
	    
	    intermediate::RegisterPtr seqRegID (exp->second);
	    //outputParamFreqs.push_back (seqSplit.GetLocalRegFreqFlags (seqRegID));
	    #endif
	    outputParamFreqs.push_back (freqFlagF);
	  }
	  newFunc->outputParamFreqs = outputParamFreqs;
      
	  // Generate 'split' functions
	  UnicodeString funcFName ("fragment_");
	  funcFName.append (decoratedIdent);
	  freqFuncIdents[freqFragment] = funcFName;
	  newFunc->funcName[freqFragment] = funcFName;
	  
	  // Fake all inputs to fragment frequency
	  for (size_t i = 0; i < inputParamFreqFlags.size(); i++)
	  {
	    seqSplit.SetInputFreqFlags (funcParams[i].identifier,
					(inputParamFreqFlags[i] & freqFlagU) | freqFlagF);
	  }

	  seqSplit.PerformSplit();
	  
	  // Generate 'split' functions
	  parser::SemanticsHandler::Scope::FunctionFormalParameters extraParamsF;
	  AddFreqFunction (funcFName, progFunc, extraParamsF, seqSplit.GetOutputFragmentSequence(), freqFragment);
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
	  parser::SemanticsHandler::Scope::FunctionFormalParameters extraParams[freqNum];
	  const std::vector<intermediate::RegisterPtr>& transfers = seqSplit.GetTransferRegs (freqVertex);
	  unsigned int n = 0;
	  BOOST_FOREACH (const intermediate::RegisterPtr& reg, transfers)
	  {
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
	      paramV.type = reg->GetOriginalType();
	      extraParams[freqVertex].push_back (paramV);
	      
	      seqSplit.GetOutputVertexSequence()->SetExport (transferIdent, reg);
	    }
	    {
	      parser::SemanticsHandler::Scope::FunctionFormalParameter paramF;
	      paramF.dir = parser::SemanticsHandler::Scope::dirIn;
	      paramF.identifier = transferIdent;
	      paramF.type = reg->GetOriginalType();
	      extraParams[freqFragment].push_back (paramF);
	      
	      seqSplit.GetOutputFragmentSequence()->AddImport (transferIdent, reg);
	    }
	    
	    // Also return transfer info
	    FunctionTransferValues newTransferValue;
	    newTransferValue.valueType = reg->GetOriginalType();
	    transferValues[freqVertex].push_back (newTransferValue);
	  }
	  newFunc->transferValues[freqVertex] = transferValues[freqVertex];
	  
	  // Extract output parameters, to return output value frequencies
	  const intermediate::Sequence::RegisterExpMappings& seqExports = progFunc->GetBody()->GetExports();
	  BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& funcParam, funcParams)
	  {
	    if ((int (funcParam.dir) & parser::SemanticsHandler::Scope::dirOut) == 0) continue;
	    
	    intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (funcParam.identifier);
	    if (exp == seqExports.end())
	    {
	      // Output param is never written...
	      outputParamFreqs.push_back (freqFlagU);
	    }
	    else
	    {
	      intermediate::RegisterPtr seqRegID (exp->second);
	      outputParamFreqs.push_back (seqSplit.GetRegAvailability (seqRegID));
	    }
	  }
	  newFunc->outputParamFreqs = outputParamFreqs;
      
	  // Generate 'split' functions
	  for (int f = 0; f < freqNum; f++)
	  {
	    intermediate::SequencePtr seq (seqSplit.GetOutputSequence (f));
	    if (!(seq->GetNumOps() > 0) || (extraParams[f].size() > 0))
	      continue;
	    
	    static const char* const freqPrefix[freqNum] = { "uniform_", "vertex_", "fragment_" };
	      
	    UnicodeString funcName (freqPrefix[f]);
	    funcName.append (decoratedIdent);
	    AddFreqFunction (funcName, progFunc, extraParams[f], seq, f);
	    freqFuncIdents[f] = funcName;
	    newFunc->funcName[f] = funcName;
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
					   const intermediate::SequencePtr& sequence,
					   int freq)
    {
      parser::SemanticsHandler::Scope::FunctionFormalParameters params (originalFunc->GetParams());
      params.insert (params.end(), extraParams.begin(), extraParams.end());
      
      intermediate::ProgramFunctionPtr newFunc (boost::make_shared<intermediate::ProgramFunction> (originalFunc->GetOriginalIdentifier(),
												   funcName,
												   params,
												   sequence,
												   false));
      outputPrograms[freq]->AddFunction (newFunc);
    }
    
    class ProgramSplitter::RecursionChecker : public intermediate::SequenceVisitor
    {
      ProgramSplitter& parent;
      UnicodeString funcToCheck;
      bool recursionFound;
      
      boost::unordered_set<UnicodeString> seenFunctions;
    public:
      RecursionChecker (ProgramSplitter& parent, const UnicodeString& funcToCheck)
       : parent (parent), funcToCheck (funcToCheck), recursionFound (false) {}
      
      bool WasRecursionFound() const { return recursionFound; }
      
      void SetVisitedOp (const intermediate::SequenceOpPtr& op) {}
      
      typedef intermediate::RegisterPtr RegisterPtr;
      void OpConstBool (const RegisterPtr&, bool) {}
      void OpConstInt (const RegisterPtr&, int) {}
      void OpConstUInt (const RegisterPtr&, unsigned int) {}
      void OpConstFloat (const RegisterPtr&, float) {}
				
      void OpAssign (const RegisterPtr&, const RegisterPtr&) {}
				
      void OpCast (const RegisterPtr&, intermediate::BasicType, const RegisterPtr&) {}
				  
      void OpMakeVector (const RegisterPtr&, intermediate::BasicType, const std::vector<RegisterPtr>&) {}

      void OpMakeMatrix (const RegisterPtr&, intermediate::BasicType, unsigned int, unsigned int,
			 const std::vector<RegisterPtr>&) {}

      void OpMakeArray (const RegisterPtr&, const std::vector<RegisterPtr>&) {}
      void OpExtractArrayElement (const RegisterPtr&, const RegisterPtr&, const RegisterPtr&) {}
      void OpChangeArrayElement (const RegisterPtr&, const RegisterPtr&, const RegisterPtr&, const RegisterPtr&) {}
      void OpGetArrayLength (const RegisterPtr&, const RegisterPtr&) {}

      void OpExtractVectorComponent (const RegisterPtr&, const RegisterPtr&, unsigned int) {}
				
      void OpArith (const RegisterPtr&, ArithmeticOp, const RegisterPtr&, const RegisterPtr&) {}
      void OpCompare (const RegisterPtr&, CompareOp, const RegisterPtr&, const RegisterPtr&) {}
      void OpLogic (const RegisterPtr&, LogicOp, const RegisterPtr&, const RegisterPtr&) {}
      void OpUnary (const RegisterPtr&, UnaryOp, const RegisterPtr&) {}
			      
      void OpBlock (const intermediate::SequencePtr& subSequence,
		    const intermediate::Sequence::IdentifierToRegMap& identToRegIDs_imp,
		    const intermediate::Sequence::IdentifierToRegMap& identToRegIDs_exp)
      {
	if (recursionFound) return;
	subSequence->Visit (*this);
      }
			    
      void OpBranch (const RegisterPtr& conditionReg,
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
      
      void OpWhile (const RegisterPtr& conditionReg,
		    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
		    const intermediate::SequenceOpPtr& seqOpBody)
      {
	if (recursionFound) return;
	boost::shared_ptr<intermediate::SequenceOpBlock> block (
	  boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpBody));
	assert (block);
	block->GetSequence()->Visit (*this);
      }
			    
      void OpReturn (const std::vector<RegisterPtr>&) {}
      void OpFunctionCall (const UnicodeString& funcIdent,
			   const std::vector<RegisterPtr>& inParams,
			   const std::vector<RegisterPtr>& outParams)
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
      
      void OpBuiltinCall (const RegisterPtr&, intermediate::BuiltinFunction, const std::vector<RegisterPtr>&) {}
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
      for (int f = 0; f < freqNum; f++)
      {
	outputPrograms[f] = boost::make_shared<intermediate::Program> ();
      }
      const intermediate::Program::OutputParameters& progOutput = inputProgram->GetOutputParameters();
      {
	for (intermediate::Program::OutputParameters::const_iterator outParam = progOutput.begin();
	     outParam != progOutput.end();
	     ++outParam)
	{
	  int f = intermediate::Program::GetTargetFrequency (outParam->second);
	  outputPrograms[f]->SetOutputParameter (outParam->first, outParam->second);
	}
      }
  
      for (size_t i = 0; i < inputProgram->GetNumFunctions(); i++)
      {
	intermediate::ProgramFunctionPtr func = inputProgram->GetFunction (i);
	if (!func->IsEntryFunction()) continue;
	
	splitter::SequenceSplitter splitter (*this);
	splitter.SetInputSequence (func->GetBody());
	
	parser::SemanticsHandler::Scope::FunctionFormalParameters vParams;
	parser::SemanticsHandler::Scope::FunctionFormalParameters fParams;
	typedef std::pair<UnicodeString, int> ParamFreqPair;
	std::vector<ParamFreqPair> allFrequencies;
	BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& param, func->GetParams())
	{
	  if (param.dir & parser::SemanticsHandler::Scope::dirIn)
	  {
	    // Input parameter
	    int paramFreq;
	    ParamMap::const_iterator paramFlag = paramFlags.find (param.identifier);
	    if (paramFlag != paramFlags.end())
	    {
	      paramFreq = paramFlag->second;
	    }
	    else
	    {
	      paramFreq = 1 << SequenceSplitter::GetDefaultFrequencyForType (param.type);
	    }
	    splitter.SetInputFreqFlags (param.identifier, paramFreq);
	    if ((paramFreq & (freqFlagU | freqFlagV)) != 0)
	      vParams.push_back (param);
	    if ((paramFreq & (freqFlagU | freqFlagF)) != 0)
	      fParams.push_back (param);
	    
	    allFrequencies.push_back (std::make_pair (param.identifier, paramFreq));
	  }
	  else
	  {
	    // Output parameter
	    // Filter all except vertex/fragment out
	    intermediate::Program::OutputParameters::const_iterator outParam = progOutput.find (param.identifier);
	    if (outParam != progOutput.end())
	    {
	      switch (intermediate::Program::GetTargetFrequency (outParam->second))
	      {
	      case freqVertex:
		vParams.push_back (param);
		break;
	      case freqFragment:
		fParams.push_back (param);
		break;
	      }
	    }
	  }
	}
	
	splitter.PerformSplit();
	
	UnicodeString funcVName ("vertex_");
	//funcVName.append (func->GetIdentifier());
	funcVName.append ("main");
	
	intermediate::ProgramFunctionPtr funcV (boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier(),
												   funcVName,
												   vParams,
												   splitter.GetOutputVertexSequence(),
												   func->IsEntryFunction()));
	outputPrograms[freqVertex]->AddFunction (funcV);

	UnicodeString funcFName ("fragment_");
	//funcFName.append (func->GetIdentifier());
	funcFName.append ("main");
	
	intermediate::ProgramFunctionPtr funcF (boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier(),
												   funcFName,
												   fParams,
												   splitter.GetOutputFragmentSequence(),
												   func->IsEntryFunction()));
	outputPrograms[freqFragment]->AddFunction (funcF);
	
	const std::vector<intermediate::RegisterPtr>& transferV2F = splitter.GetTransferRegs (freqVertex);
	BOOST_FOREACH(const intermediate::RegisterPtr& reg, transferV2F)
	{
	  outputPrograms[freqVertex]->AddTransferValue (reg->GetOriginalType(), reg->GetName());
	  funcV->SetTransferMapping (reg->GetName(), reg);
	  funcF->SetTransferMapping (reg->GetName(), reg);
	}
	
	BOOST_FOREACH(const ParamFreqPair& pfp, allFrequencies)
	{
	  funcV->SetParameterFrequency (pfp.first, pfp.second);
	  funcF->SetParameterFrequency (pfp.first, pfp.second);
	}
      }
    }
    
    const intermediate::ProgramPtr& ProgramSplitter::GetOutputProgram (int freq)
    {
      return outputPrograms[freq];
    }
    
  } // namespace splitter
} // namespace s1

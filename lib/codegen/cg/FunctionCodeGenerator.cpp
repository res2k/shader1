#include <boost/cstdint.hpp>

#include "FunctionCodeGenerator.h"

#include "base/unordered_set"
#include "intermediate/ProgramFunction.h"
#include "splitter/Frequency.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
    void CgGenerator::FunctionCodeGenerator::ParamAdder::Add (const char* attr, const std::string& attrStr)
    {
      if (!firstParam)
	paramStr.append (", ");
      else
	firstParam = false;
      paramStr.append (attr);
      paramStr.append (attrStr);
    }
	  
    StringsArrayPtr CgGenerator::FunctionCodeGenerator::Generate (const intermediate::ProgramFunctionPtr& func,
								  const UnicodeString& vertexOutput,
								  const UnicodeString& fragmentOutput,
								  bool doTransfer)
    {
      typedef parser::SemanticsHandler::Scope::FunctionFormalParameters FunctionFormalParameters;
      
      BlockNameResolver nameRes;
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      {
	std::string typeSuffix;
	std::string typeStr (TypeToCgType (func->GetReturnType(), typeSuffix));
	std::string identifier (NameToCgIdentifier (func->GetIdentifier()));
	
	std::string funcDecl (typeStr);
	funcDecl.append (" ");
	funcDecl.append (identifier);
	funcDecl.append (" (");
	
	const intermediate::ProgramFunction::ParameterFrequencyMap& paramFreqs (func->GetParameterFrequencies());
	
	std::tr1::unordered_set<UnicodeString> paramImports;
	std::vector<std::pair<std::string, UnicodeString> > inParams;
	std::vector<std::string> outParams;
	const FunctionFormalParameters& params (func->GetParams());
	for (FunctionFormalParameters::const_iterator param = params.begin();
	     param != params.end();
	     ++param)
	{
	  paramImports.insert (param->identifier);
	  
	  std::string typeSuffix;
	  std::string paramStrBase;
	  paramStrBase.append (TypeToCgType (param->type, typeSuffix));
	  paramStrBase.append (" ");

	  if (param->dir & parser::SemanticsHandler::Scope::dirIn)
	  {
	    UnicodeString paramIdentDecorated ("i");
	    paramIdentDecorated.append (param->identifier);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    std::string paramStr (paramStrBase);
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    inParams.push_back (std::make_pair (paramStr, param->identifier));
	    
	    nameRes.inParamMap[param->identifier] = paramIdent;
	  }
	  
	  if (param->dir & parser::SemanticsHandler::Scope::dirOut)
	  {
	    UnicodeString paramIdentDecorated ("o");
	    paramIdentDecorated.append (param->identifier);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    std::string paramStr (paramStrBase);
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    if (param->identifier == vertexOutput)
	      paramStr.append (" : POSITION");
	    else if (param->identifier == fragmentOutput)
	      paramStr.append (" : COLOR");
	    outParams.push_back (paramStr);
	    
	    nameRes.outParamMap[param->identifier] = paramIdent;
	  }
	}
	
	if (!func->IsEntryFunction ()) // In entry functions, global vars are actually local
	{
	  // Generate parameters for sequence imports (ie globals)
	  intermediate::Sequence::RegisterImpMappings imports (func->GetBody()->GetImports());
	  for (intermediate::Sequence::RegisterImpMappings::const_iterator import (imports.begin());
	      import != imports.end();
	      ++import)
	  {
	    // Only look at globals
	    if (paramImports.find (import->first) != paramImports.end()) continue;
	    
	    UnicodeString paramIdentDecorated ("I");
	    paramIdentDecorated.append (import->first);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    intermediate::Sequence::RegisterBankPtr bank;
	    func->GetBody()->QueryRegisterPtrFromID (import->second, bank);
	    std::string typeSuffix;
	    std::string paramStr (TypeToCgType (bank->GetOriginalType(), typeSuffix));
	    paramStr.append (" ");
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    inParams.push_back (std::make_pair (paramStr, import->first));
	    
	    nameRes.inParamMap[import->first] = paramIdent;
	  }
	  
	  // Generate parameters for sequence exports
	  intermediate::Sequence::RegisterExpMappings exports (func->GetBody()->GetExports());
	  for (intermediate::Sequence::RegisterExpMappings::const_iterator exported (exports.begin());
	      exported != exports.end();
	      ++exported)
	  {
	    // Only look at globals
	    if (paramImports.find (exported->first) != paramImports.end()) continue;
	    
	    UnicodeString paramIdentDecorated ("O");
	    paramIdentDecorated.append (exported->first);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    intermediate::Sequence::RegisterBankPtr bank;
	    func->GetBody()->QueryRegisterPtrFromID (exported->second, bank);
	    std::string typeSuffix;
	    std::string paramStr (TypeToCgType (bank->GetOriginalType(), typeSuffix));
	    paramStr.append (" ");
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    outParams.push_back (paramStr);
	    
	    nameRes.outParamMap[exported->first] = paramIdent;
	  }
	}
	
	ParamAdder paramAdder;
	
	if (func->IsEntryFunction () && doTransfer)
	{
	  if (func->GetExecutionFrequency() == splitter::freqVertex)
	    paramAdder.Add ("out ", "V2F v2f");
	  else
	    paramAdder.Add ("in ", "V2F v2f");
	}
	
	for (std::vector<std::pair<std::string, UnicodeString> >::const_iterator inParam (inParams.begin());
	     inParam != inParams.end();
	     ++inParam)
	{
	  const char* variability = "in ";
	  intermediate::ProgramFunction::ParameterFrequencyMap::const_iterator pf = paramFreqs.find (inParam->second);
	  if (pf != paramFreqs.end())
	  {
	    if (pf->second & splitter::freqFlagU)
	      variability = "uniform in ";
	    else if (pf->second & (splitter::freqFlagV | splitter::freqFlagF))
	      variability = "varying in ";
	  }
	  
	  paramAdder.Add (variability, inParam->first);
	}
	for (std::vector<std::string>::const_iterator outParam (outParams.begin());
	     outParam != outParams.end();
	     ++outParam)
	{
	  paramAdder.Add ("out ", *outParam);
	}
	
	funcDecl.append (paramAdder.paramStr);
	funcDecl.append (")");
	funcDecl.append (typeSuffix); // FIXME: Right?
	resultStrings->AddString (funcDecl);
      }
      resultStrings->AddString (std::string ("{"));

      intermediate::ProgramFunction::TransferMappings emptyTransfer;
      const intermediate::ProgramFunction::TransferMappings* transferIn = &emptyTransfer;
      const intermediate::ProgramFunction::TransferMappings* transferOut = &emptyTransfer;
      
	if (func->IsEntryFunction () && doTransfer)
	{
	  if (func->GetExecutionFrequency() == splitter::freqVertex)
	    transferOut = &(func->GetTransferMappings());
	  else
	    transferIn = &(func->GetTransferMappings());
	}
      
      SequenceCodeGenerator seqGen (*(func->GetBody()), &nameRes, *transferIn, *transferOut);
      resultStrings->AddStrings (*(seqGen.Generate()), 2);
      
      resultStrings->AddString (std::string ("}"));
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1


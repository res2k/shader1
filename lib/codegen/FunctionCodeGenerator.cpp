#include <boost/cstdint.hpp>

#include "FunctionCodeGenerator.h"

#include "base/unordered_set"
#include "intermediate/ProgramFunction.h"
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
	  
    StringsArrayPtr CgGenerator::FunctionCodeGenerator::Generate (const intermediate::ProgramFunctionPtr& func)
    {
      typedef parser::SemanticsHandler::Scope::FunctionFormalParameters FunctionFormalParameters;
      
      BlockNameResolver nameRes;
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      {
	std::string typeStr (TypeToCgType (func->GetReturnType()));
	std::string identifier (NameToCgIdentifier (func->GetIdentifier()));
	
	std::string funcDecl (typeStr);
	funcDecl.append (" ");
	funcDecl.append (identifier);
	funcDecl.append (" (");
	
	std::tr1::unordered_set<UnicodeString> paramImports;
	std::vector<std::string> inParams;
	std::vector<std::string> outParams;
	const FunctionFormalParameters& params (func->GetParams());
	for (FunctionFormalParameters::const_iterator param = params.begin();
	     param != params.end();
	     ++param)
	{
	  paramImports.insert (param->identifier);
	  
	  std::string paramStrBase;
	  paramStrBase.append (TypeToCgType (param->type));
	  paramStrBase.append (" ");

	  if (param->dir & parser::SemanticsHandler::Scope::dirIn)
	  {
	    UnicodeString paramIdentDecorated ("i");
	    paramIdentDecorated.append (param->identifier);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    std::string paramStr (paramStrBase);
	    paramStr.append (paramIdent);
	    inParams.push_back (paramStr);
	    
	    nameRes.inParamMap[param->identifier] = paramIdent;
	  }
	  
	  if (param->dir & parser::SemanticsHandler::Scope::dirOut)
	  {
	    UnicodeString paramIdentDecorated ("o");
	    paramIdentDecorated.append (param->identifier);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    std::string paramStr (paramStrBase);
	    paramStr.append (paramIdent);
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
	    std::string paramStr (TypeToCgType (bank->GetOriginalType()));
	    paramStr.append (" ");
	    paramStr.append (paramIdent);
	    inParams.push_back (paramStr);
	    
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
	    std::string paramStr (TypeToCgType (bank->GetOriginalType()));
	    paramStr.append (" ");
	    paramStr.append (paramIdent);
	    outParams.push_back (paramStr);
	    
	    nameRes.outParamMap[exported->first] = paramIdent;
	  }
	}
	
	ParamAdder paramAdder;
	for (std::vector<std::string>::const_iterator inParam (inParams.begin());
	     inParam != inParams.end();
	     ++inParam)
	{
	  paramAdder.Add ("in ", *inParam);
	}
	for (std::vector<std::string>::const_iterator outParam (outParams.begin());
	     outParam != outParams.end();
	     ++outParam)
	{
	  paramAdder.Add ("out ", *outParam);
	}
	
	funcDecl.append (paramAdder.paramStr);
	funcDecl.append (")");
	resultStrings->AddString (funcDecl);
      }
      resultStrings->AddString (std::string ("{"));
      
      SequenceCodeGenerator seqGen (*(func->GetBody()), &nameRes);
      resultStrings->AddStrings (*(seqGen.Generate()), 2);
      
      resultStrings->AddString (std::string ("}"));
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1


#include "base/common.h"

#include "FunctionCodeGenerator.h"

#include "intermediate/ProgramFunction.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
    void LatexGenerator::FunctionCodeGenerator::ParamAdder::Add (const char* attr, const std::string& attrStr)
    {
      if (!firstParam)
	paramStr.append (", ");
      else
	firstParam = false;
      paramStr.append (attr);
      paramStr.append (attrStr);
    }
	  
    StringsArrayPtr LatexGenerator::FunctionCodeGenerator::Generate (const intermediate::ProgramFunctionPtr& func)
    {
      typedef parser::SemanticsHandler::Scope::FunctionFormalParameters FunctionFormalParameters;
      
      BlockNameResolver nameRes;
      SequenceCodeGenerator seqGen (*(func->GetBody()), &nameRes);
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      resultStrings->AddString ("\\sOfunc{");
      {
	std::string identifier;
	func->GetOriginalIdentifier().toUTF8String (identifier);
	
	{
	  std::string funcDecl ("\\sOfunchead{");
	  funcDecl.append (LatexEscape (identifier));
	  funcDecl.append ("}");
	  resultStrings->AddString (funcDecl);
	}
	
	const intermediate::Sequence::RegisterImpMappings& imports = func->GetBody()->GetImports();
	const intermediate::Sequence::RegisterExpMappings& exports = func->GetBody()->GetExports();
	
	boost::unordered_set<UnicodeString> paramImports;
	std::vector<std::string> inParams;
	std::vector<std::string> outParams;
	const FunctionFormalParameters& params (func->GetParams());
	for (FunctionFormalParameters::const_iterator param = params.begin();
	     param != params.end();
	     ++param)
	{
	  paramImports.insert (param->identifier);
	  
	  std::string paramStr ("{");
	  paramStr.append (TypeString (param->type));
	  paramStr.append ("}{");
	  {
	    std::string paramIdent;
	    param->identifier.toUTF8String (paramIdent);
	    paramStr.append (paramIdent);
	  }
	  paramStr.append ("}");
	  
	  if (param->dir & parser::SemanticsHandler::Scope::dirIn)
	  {
	    std::string paramStrIn ("\\sOparamIn");
	    paramStrIn.append (paramStr);
	    paramStrIn.append ("{");
	    for (intermediate::Sequence::RegisterImpMappings::const_iterator import = imports.begin();
		  import != imports.end();
		  ++import)
	    {
	      if (import->first.compare (param->identifier) == 0)
	      {
		paramStrIn.append (seqGen.GetOutputRegisterName (import->second));
		break;
	      }
	    }
	    paramStrIn.append ("}");
	    inParams.push_back (paramStrIn);
	    
	    nameRes.inParamMap[param->identifier] = paramStrIn;
	  }
	  
	  if (param->dir & parser::SemanticsHandler::Scope::dirOut)
	  {
	    std::string paramStrOut ("\\sOparamOut");
	    paramStrOut.append (paramStr);
	    for (intermediate::Sequence::RegisterExpMappings::const_iterator exportVar = exports.begin();
		  exportVar != exports.end();
		  ++exportVar)
	    {
	      if (exportVar->first.compare (param->identifier) == 0)
	      {
		paramStrOut.append ("{");
		paramStrOut.append (seqGen.GetOutputRegisterName (exportVar->second));
		paramStrOut.append ("}");
		break;
	      }
	    }
	    outParams.push_back (paramStrOut);
	    
	    nameRes.outParamMap[param->identifier] = paramStrOut;
	  }
	}

#if 0
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
	    std::string typeSuffix;
	    std::string paramStr (TypeToCgType (bank->GetOriginalType(), typeSuffix));
	    paramStr.append (" ");
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    outParams.push_back (paramStr);
	    
	    nameRes.outParamMap[exported->first] = paramIdent;
	  }
	}
#endif
	
	if (inParams.size() > 0)
	{
	  std::string inParamsLine ("\\sOfuncinparams{");
	  for (std::vector<std::string>::const_iterator inParam (inParams.begin());
	      inParam != inParams.end();
	      ++inParam)
	  {
	    inParamsLine.append (*inParam);
	  }
	  inParamsLine.append ("}");
	  resultStrings->AddString (inParamsLine);
	}
	if (outParams.size() > 0)
	{
	  std::string outParamsLine ("\\sOfuncoutparams{");
	  for (std::vector<std::string>::const_iterator outParam (outParams.begin());
	      outParam != outParams.end();
	      ++outParam)
	  {
	    outParamsLine.append (*outParam);
	  }
	  outParamsLine.append ("}");
	  resultStrings->AddString (outParamsLine);
	}
      }
      
      StringsArrayPtr bodyStrings (seqGen.Generate());
      
      for (SequenceCodeGenerator::RegisterSets::const_iterator regSet (seqGen.registerSets.begin());
	   regSet != seqGen.registerSets.end();
	   ++regSet)
      {
	std::string regSetStr ("\\sOfuncregset{");
	regSetStr.append (LatexEscape (regSet->first));
	regSetStr.append ("}{");
	for (SequenceCodeGenerator::RegisterNameSet::const_iterator regs (regSet->second.begin());
	     regs != regSet->second.end();
	     ++regs)
	{
	  std::string regStr;
	  regs->toUTF8String (regStr);
	  regSetStr.append ("\\sOsetreg{");
	  regSetStr.append (LatexEscape (regStr));
	  regSetStr.append ("}");
	}
	regSetStr.append ("}");
	resultStrings->AddString (regSetStr);
      }
      
      resultStrings->AddString (std::string ("\\sOfuncbody{"));
      resultStrings->AddStrings (*bodyStrings, 2, indentStr);
      resultStrings->AddString (std::string ("}"));
      
      resultStrings->AddString (std::string ("}"));
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1


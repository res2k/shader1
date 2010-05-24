#include <boost/cstdint.hpp>

#include "FunctionCodeGenerator.h"

#include "intermediate/ProgramFunction.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
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
	
	std::vector<std::string> inParams;
	std::vector<std::string> outParams;
	const FunctionFormalParameters& params (func->GetParams());
	for (FunctionFormalParameters::const_iterator param = params.begin();
	     param != params.end();
	     ++param)
	{
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
	
	bool firstParam = true;
	for (std::vector<std::string>::const_iterator inParam (inParams.begin());
	     inParam != inParams.end();
	     ++inParam)
	{
	  if (!firstParam)
	    funcDecl.append (", ");
	  else
	    firstParam = false;
	  funcDecl.append ("in ");
	  funcDecl.append (*inParam);
	}
	for (std::vector<std::string>::const_iterator outParam (outParams.begin());
	     outParam != outParams.end();
	     ++outParam)
	{
	  if (!firstParam)
	    funcDecl.append (", ");
	  else
	    firstParam = false;
	  funcDecl.append ("out ");
	  funcDecl.append (*outParam);
	}
	
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


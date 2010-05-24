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
      
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      {
	std::string typeStr (TypeToCgType (func->GetReturnType()));
	std::string identifier (NameToCgIdentifier (func->GetIdentifier()));
	
	std::string funcDecl (typeStr);
	funcDecl.append (" ");
	funcDecl.append (identifier);
	funcDecl.append (" (");
	bool firstParam = true;
	const FunctionFormalParameters& params (func->GetParams());
	for (FunctionFormalParameters::const_iterator param = params.begin();
	     param != params.end();
	     ++param)
	{
	  std::string paramStr;
	  if (!firstParam)
	    paramStr = ", ";
	  else
	    firstParam = false;
	  paramStr.append (TypeToCgType (param->type));
	  paramStr.append (" ");
	  paramStr.append (NameToCgIdentifier (param->identifier));
	  funcDecl.append (paramStr);
	}
	funcDecl.append (")");
	resultStrings->AddString (funcDecl);
      }
      resultStrings->AddString (std::string ("{"));
      
      BlockNameResolver nameRes;
      SequenceCodeGenerator seqGen (*(func->GetBody()), &nameRes);
      resultStrings->AddStrings (*(seqGen.Generate()), 2);
      
      resultStrings->AddString (std::string ("}"));
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1


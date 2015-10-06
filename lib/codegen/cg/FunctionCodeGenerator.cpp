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

#include "base/common.h"

#include "FunctionCodeGenerator.h"

#include "intermediate/ProgramFunction.h"
#include "splitter/Frequency.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>
#include <sstream>

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
	  
    StringsArrayPtr CgGenerator::FunctionCodeGenerator::Generate (const char* identifier,
                                                                  const intermediate::ProgramFunctionPtr& func,
								  const intermediate::Program::OutputParameters& output,
								  const intermediate::Program::ParameterArraySizes& paramArraySizes,
								  bool doTransfer,
								  int frequency)
    {
      typedef parser::SemanticsHandler::Scope Scope;
      typedef Scope::FunctionFormalParameters FunctionFormalParameters;
      
      BlockNameResolver nameRes;
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      std::vector<std::string> outParamIdents;
      {
	std::string typeSuffix;
	
	std::string funcDecl ("void ");
	funcDecl.append (identifier);
	funcDecl.append (" (");
	
	const intermediate::ProgramFunction::ParameterFrequencyMap& paramFreqs (func->GetParameterFrequencies());
	
	boost::unordered_set<uc::String> paramImports;
	std::vector<std::pair<std::string, uc::String> > inParams;
	std::vector<std::string> outParams;
	const FunctionFormalParameters& params (func->GetParams());
	for (FunctionFormalParameters::const_iterator param = params.begin();
	     param != params.end();
	     ++param)
	{
	  paramImports.insert (param->identifier);
	  
	  std::string sizeStr;
	  {
	    intermediate::Program::ParameterArraySizes::const_iterator paramSize (paramArraySizes.find (param->identifier));
	    if (paramSize != paramArraySizes.end())
	    {
	      std::stringstream valueStrStream;
	      valueStrStream << paramSize->second;
	      sizeStr = valueStrStream.str();
	    }
	  }
	  
	  std::string typeSuffix;
	  std::string paramStrBase;
	  paramStrBase.append (TypeToCgType (param->type, typeSuffix, sizeStr));
	  paramStrBase.append (" ");

	  if (param->dir & parser::SemanticsHandler::Scope::dirIn)
	  {
	    uc::String paramIdentDecorated (param->paramType == Scope::ptAutoGlobal ? "I" : "i");
	    paramIdentDecorated.append (param->identifier);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    std::string paramStr (paramStrBase);
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    inParams.emplace_back (paramStr, param->identifier);
	    
	    nameRes.inParamMap[param->identifier] = paramIdent;
	  }
	  
	  if (param->dir & parser::SemanticsHandler::Scope::dirOut)
	  {
	    uc::String paramIdentDecorated (param->paramType == Scope::ptAutoGlobal ? "O" : "o");
	    paramIdentDecorated.append (param->identifier);
	    std::string paramIdent (NameToCgIdentifier (paramIdentDecorated));
	    outParamIdents.push_back (paramIdent);
	    std::string paramStr (paramStrBase);
	    paramStr.append (paramIdent);
	    paramStr.append (typeSuffix);
	    intermediate::Program::OutputParameters::const_iterator outputInfo = output.find (param->identifier);
	    if (outputInfo != output.end())
	    {
	      switch (outputInfo->second)
	      {
	      case intermediate::Program::Position:
		paramStr.append (" : POSITION");
		break;
	      case intermediate::Program::Color:
		paramStr.append (" : COLOR");
		break;
	      }
	    }
	    outParams.push_back (paramStr);
	    
	    nameRes.outParamMap[param->identifier] = paramIdent;
	  }
	}
	
	ParamAdder paramAdder;
	
	if (func->IsEntryFunction () && doTransfer)
	{
	  if (frequency == splitter::freqVertex)
	    paramAdder.Add ("out ", "V2F v2f");
	  else
	    paramAdder.Add ("in ", "V2F v2f");
	}
	
	for (std::vector<std::pair<std::string, uc::String> >::const_iterator inParam (inParams.begin());
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
	  if (frequency == splitter::freqVertex)
	    transferOut = &(func->GetTransferMappings());
	  else
	    transferIn = &(func->GetTransferMappings());
	}
      
      SequenceCodeGenerator seqGen (*(func->GetBody()), &nameRes, *transferIn, *transferOut,
				    outParamIdents);
      resultStrings->AddStrings (*(seqGen.Generate()), 2);
      
      resultStrings->AddString (std::string ("}"));
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1


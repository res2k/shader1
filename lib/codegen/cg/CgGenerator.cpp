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

#include "codegen/cg/CgGenerator.h"

#include "base/format/Formatter.h"
#include "base/format/std_string.h"

#include "ProgramCodeGenerator.h"

#include <limits>
#include <stdio.h>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace codegen
  {
    const char CgGenerator::cgTypeBool[]	= "bool";
    const char CgGenerator::cgTypeInt[]		= "int";
    const char CgGenerator::cgTypeUInt[]	= "unsigned int";
    const char CgGenerator::cgTypeFloat[]	= "float";

    CgGenerator::CgGenerator ()
    {
    }
    
    StringsArrayPtr CgGenerator::Generate (const intermediate::ProgramPtr& program,
					   int frequency,
                                           const CgOptions& options)
    {
      ProgramCodeGenerator progGen (options);
      return progGen.Generate (program, frequency);
    }

    static format::StaticFormatter FormatTypeVector ("{0}{1}");
    static format::StaticFormatter FormatTypeMatrix ("{0}{1}x{2}");
    
    std::string CgGenerator::TypeToCgType (const parser::SemanticsHandler::TypePtr& type,
					   std::string& identifierSuffix,
					   const std::string& sizeStr)
    {
      identifierSuffix = "";
      std::string typeStr;
      switch (type->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
	{
	  switch (type->GetBaseType())
	  {
	  case parser::SemanticsHandler::Void:	typeStr = "void"; break;
	  case parser::SemanticsHandler::Bool:	typeStr = cgTypeBool; break;
	  case parser::SemanticsHandler::Int:	typeStr = cgTypeInt; break;
	  case parser::SemanticsHandler::UInt:	typeStr = cgTypeUInt; break;
	  case parser::SemanticsHandler::Float:	typeStr = cgTypeFloat; break;
	  }
	}
	break;
      case parser::SemanticsHandler::Type::Sampler:
	{
	  switch (type->GetSamplerType())
	  {
	  case parser::SemanticsHandler::_1D:	typeStr = "sampler1D"; break;
	  case parser::SemanticsHandler::_2D:	typeStr = "sampler2D"; break;
	  case parser::SemanticsHandler::_3D:	typeStr = "sampler3D"; break;
	  case parser::SemanticsHandler::CUBE:	typeStr = "samplerCUBE"; break;
	  }
	}
	break;
      case parser::SemanticsHandler::Type::Array:
	{
	  std::string newSuffix;
	  std::string typeStr (TypeToCgType (type->GetArrayVectorMatrixBaseType(), newSuffix));
	  identifierSuffix = newSuffix + "[" + sizeStr + "]";
	  return typeStr;
	}
	break;
      case parser::SemanticsHandler::Type::Vector:
	{
	  std::string newSuffix;
          FormatTypeVector (typeStr,
            TypeToCgType (type->GetArrayVectorMatrixBaseType(), newSuffix),
            type->GetVectorTypeComponents());
	}
	break;
      case parser::SemanticsHandler::Type::Matrix:
	{
	  std::string newSuffix;
          FormatTypeMatrix (typeStr,
            TypeToCgType (type->GetArrayVectorMatrixBaseType(), newSuffix),
            type->GetMatrixTypeRows(), type->GetMatrixTypeCols());
	}
	break;
      }
      return typeStr;
    }
  } // namespace codegen
} // namespace s1

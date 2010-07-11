#include "codegen/LatexGenerator.h"

#include "latex/ProgramCodeGenerator.h"

namespace s1
{
  namespace codegen
  {
    const char LatexGenerator::indentStr[]	= "";
    const char LatexGenerator::typeBool[]	= "bool";
    const char LatexGenerator::typeInt[]	= "int";
    const char LatexGenerator::typeUInt[]	= "unsigned int";
    const char LatexGenerator::typeFloat[]	= "float";
    
    LatexGenerator::LatexGenerator ()
    {
    }
    
    StringsArrayPtr LatexGenerator::Generate (const intermediate::ProgramPtr& program)
    {
      ProgramCodeGenerator progGen;
      return progGen.Generate (program);
    }
    
    std::string LatexGenerator::TypeString (const parser::SemanticsHandler::TypePtr& type)
    {
      std::string typeStr;
      switch (type->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
	{
	  switch (type->GetBaseType())
	  {
	  case parser::SemanticsHandler::Void:	typeStr = "void"; break;
	  case parser::SemanticsHandler::Bool:	typeStr = typeBool; break;
	  case parser::SemanticsHandler::Int:	typeStr = typeInt; break;
	  case parser::SemanticsHandler::UInt:	typeStr = typeUInt; break;
	  case parser::SemanticsHandler::Float:	typeStr = typeFloat; break;
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
	  std::string typeStr (TypeString (type->GetArrayVectorMatrixBaseType()));
	  typeStr += "[]";
	  return typeStr;
	}
	break;
      case parser::SemanticsHandler::Type::Vector:
	{
	  typeStr = TypeString (type->GetArrayVectorMatrixBaseType());
	  char compStr[2];
	  snprintf (compStr, sizeof (compStr), "%u", type->GetVectorTypeComponents());
	  typeStr.append (compStr);
	}
	break;
      case parser::SemanticsHandler::Type::Matrix:
	{
	  typeStr = TypeString (type->GetArrayVectorMatrixBaseType());
	  char compStr[4];
	  snprintf (compStr, sizeof (compStr), "%ux%u", type->GetMatrixTypeRows(), type->GetMatrixTypeCols());
	  typeStr.append (compStr);
	}
	break;
      }
      return typeStr;
    }
    
    std::string LatexGenerator::LatexEscape (const std::string& str)
    {
      std::string ret;
      for (std::string::const_iterator ch (str.begin());
	   ch != str.end();
	   ++ch)
      {
	switch (*ch)
	{
	case '_':	ret.append ("\\_"); break;
	case '$':	ret.append ("\\$"); break;
	case ' ':	ret.append ("\\ "); break;
	default:
	  ret += *ch;
	}
      }
      return ret;
    }
    
  } // namespace codegen
} // namespace s1
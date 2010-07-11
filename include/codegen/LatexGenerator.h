#ifndef __CODEGEN_LATEXGENERATOR_H__
#define __CODEGEN_LATEXGENERATOR_H__

#include "intermediate/Program.h"
#include "parser/SemanticsHandler.h"

#include "StringsArray.h"

namespace s1
{
  namespace codegen
  {
    class LatexGenerator
    {
    public:
      LatexGenerator ();
      
      StringsArrayPtr Generate (const intermediate::ProgramPtr& program);
      
      static std::string TypeString (const parser::SemanticsHandler::TypePtr& type);
      static std::string LatexEscape (const std::string& str);
    protected:
      class ProgramCodeGenerator;
      class FunctionCodeGenerator;
      class SequenceCodeGenerator;
      
      static const char indentStr[];
      
      static const char typeBool[];
      static const char typeInt[];
      static const char typeUInt[];
      static const char typeFloat[];
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_LATEXGENERATOR_H__

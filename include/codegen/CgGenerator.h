#ifndef __CODEGEN_CGGENERATOR_H__
#define __CODEGEN_CGGENERATOR_H__

#include "intermediate/Program.h"
#include "parser/SemanticsHandler.h"

#include "StringsArray.h"

namespace s1
{
  namespace codegen
  {
    class CgGenerator
    {
    public:
      CgGenerator ();
      
      StringsArrayPtr Generate (const intermediate::ProgramPtr& program);
      
      /**\name Code generation helper functions
       * @{ */
      static std::string TypeToCgType (const parser::SemanticsHandler::TypePtr& type);
      static std::string NameToCgIdentifier (const UnicodeString& str);
      /** @} */
    protected:
      class ProgramCodeGenerator;
      class FunctionCodeGenerator;
      class SequenceCodeGenerator;
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_CGGENERATOR_H__

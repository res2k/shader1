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
      
      StringsArrayPtr Generate (const intermediate::ProgramPtr& program,
				int frequency);
      
      /**\name Code generation helper functions
       * @{ */
      static std::string TypeToCgType (const parser::SemanticsHandler::TypePtr& type,
				       std::string& identifierSuffix,
				       const std::string& sizeStr = std::string());
      static std::string NameToCgIdentifier (const UnicodeString& str);
      /** @} */
    protected:
      class ProgramCodeGenerator;
      class FunctionCodeGenerator;
      class SequenceCodeGenerator;
      
      static const char cgTypeBool[];
      static const char cgTypeInt[];
      static const char cgTypeUInt[];
      static const char cgTypeFloat[];
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_CGGENERATOR_H__

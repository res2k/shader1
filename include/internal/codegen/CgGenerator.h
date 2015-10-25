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

#ifndef __CODEGEN_CGGENERATOR_H__
#define __CODEGEN_CGGENERATOR_H__

#include "intermediate/Program.h"
#include "parser/SemanticsHandler.h"

#include "common/StringsArray.h"

namespace s1
{
  namespace codegen
  {
    class CgOptions;

    class CgGenerator
    {
    public:
      CgGenerator ();
      
      StringsArrayPtr Generate (const intermediate::ProgramPtr& program,
				int frequency,
                                const CgOptions& options);
      
      /**\name Code generation helper functions
       * @{ */
      static std::string TypeToCgType (const parser::SemanticsHandler::TypePtr& type,
				       std::string& identifierSuffix,
				       const std::string& sizeStr = std::string());
      static std::string NameToCgIdentifier (const uc::String& str);
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

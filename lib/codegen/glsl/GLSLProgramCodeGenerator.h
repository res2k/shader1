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

#ifndef __CODEGEN_GLSL_PROGRAMCODEGENERATOR_H__
#define __CODEGEN_GLSL_PROGRAMCODEGENERATOR_H__

#include "codegen/sl/SLProgramCodeGenerator.h"
#include "intermediate/Program.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      class Options;
      class Traits;

      class ProgramCodeGenerator : public sl::ProgramCodeGenerator
      {
        const Options& GetOptions () const;

        StringsArrayPtr GeneratePreamble (const intermediate::ProgramPtr& prog, int frequency) override;
        void GenerateEntryFunctionInputs (StringsArray& strings, int progFreq, const intermediate::ProgramPtr& prog,
                                          intermediate::ProgramFunctionPtr& func);
        std::unique_ptr<sl::FunctionCodeGenerator> CreateFunctionCodeGenerator () override;
      public:
        ProgramCodeGenerator (const Traits& traits, const Options& options);
      };
    } // namespace glsl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_GLSL_PROGRAMCODEGENERATOR_H__

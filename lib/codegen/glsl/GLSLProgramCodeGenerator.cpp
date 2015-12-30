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

#include "GLSLProgramCodeGenerator.h"

#include "GLSLTraits.h"
#include "GLSLFunctionCodeGenerator.h"

#include "codegen/glsl/GLSLOptions.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      ProgramCodeGenerator::ProgramCodeGenerator (const Traits& traits, const Options& options)
        : sl::ProgramCodeGenerator (traits, options)
      {
      }

      const Options& ProgramCodeGenerator::GetOptions () const
      {
        return static_cast<const Options&> (options);
      }
      
      StringsArrayPtr ProgramCodeGenerator::GeneratePreamble (const intermediate::ProgramPtr& prog, int frequency)
      {
        // TODO: Generate globals for values passed from VP to FP
        return StringsArrayPtr ();
      }

      std::unique_ptr<sl::FunctionCodeGenerator> ProgramCodeGenerator::CreateFunctionCodeGenerator ()
      {
        return std::unique_ptr<sl::FunctionCodeGenerator> (
          new FunctionCodeGenerator (traits, GetOptions ()));
      }
    } // namespace glsl
  } // namespace codegen
} // namespace s1

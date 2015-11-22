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

#include "BackendGLSL.h"

#include "codegen/glsl/Generator.h"
#include "codegen/glsl/Options.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace compiler
  {
    BackendGLSL::BackendGLSL (Library* lib) : Backend (lib)
    {
    }
    
    Compiler::Backend::OptionsPtr BackendGLSL::CreateOptions ()
    {
      return new GLSLOptions (GetLibrary());
    }

    Compiler::Backend::ProgramPtr BackendGLSL::GenerateProgram (CompileTarget target,
                                                                const intermediate::ProgramPtr& prog,
                                                                OptionsPtr options)
    {
      int freq = -1;
      switch (target)
      {
      case targetVP:	freq = splitter::freqVertex; 	break;
      case targetFP:	freq = splitter::freqFragment; 	break;
      default:						break;
      }

      boost::intrusive_ptr<GLSLOptions> realOptions;
      if (options)
      {
        // If options are provided, verify type
        realOptions = boost::dynamic_pointer_cast<GLSLOptions> (options);
        if (!realOptions)
        {
          // TODO: Throw/err with proper error code
          return ProgramPtr ();
        }
      }
      
      codegen::glsl::Generator codegen;
      codegen::StringsArrayPtr outputProg (
        codegen.Generate (prog, freq,
                          realOptions ? realOptions->GetContainer() : codegen::glsl::Options()));
      
      return ProgramPtr (new GLSLProgram (GetLibrary(), FlattenStringArray (outputProg)));
    }

  } // namespace compiler
} // namespace s1

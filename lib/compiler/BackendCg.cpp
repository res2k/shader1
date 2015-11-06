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

#include "BackendCg.h"

#include "codegen/cg/CgGenerator.h"
#include "codegen/cg/CgOptions.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace compiler
  {
    BackendCg::BackendCg (Library* lib) : Backend (lib)
    {
    }
    
    static std::string FlattenStringArray (const codegen::StringsArrayPtr& strings)
    {
      std::string str;
      for (size_t i = 0; i < strings->Size(); i++)
      {
	str.append (strings->Get (i));
	str.append ("\n");
      }
      return str;
    }
    
    Compiler::Backend::OptionsPtr BackendCg::CreateOptions ()
    {
      return new CgOptions (GetLibrary());
    }

    Compiler::Backend::ProgramPtr BackendCg::GenerateProgram (CompileTarget target,
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

      boost::intrusive_ptr<CgOptions> realOptions;
      if (options)
      {
        // If options are provided, verify type
        realOptions = boost::dynamic_pointer_cast<CgOptions> (options);
        if (!realOptions)
        {
          // TODO: Throw/err with proper error code
          return ProgramPtr ();
        }
      }
      
      codegen::CgGenerator codegen;
      codegen::StringsArrayPtr outputProg (
        codegen.Generate (prog, freq,
                          realOptions ? realOptions->GetContainer() : codegen::CgOptions()));
      
      return ProgramPtr (new CgProgram (GetLibrary(), FlattenStringArray (outputProg)));
    }
    
    //-----------------------------------------------------------------------
    
    const std::string& BackendCg::CgProgram::GetProgramString ()
    {
      return programString;
    }

  } // namespace compiler
} // namespace s1

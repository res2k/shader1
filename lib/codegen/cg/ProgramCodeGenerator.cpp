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

#include "ProgramCodeGenerator.h"

#include "CgTraits.h"
#include "FunctionCodeGenerator.h"

#include "codegen/cg/CgOptions.h"

#if 0
#include "intermediate/Program.h"
#include "FunctionCodeGenerator.h"
#include "splitter/Frequency.h"

#include <boost/make_shared.hpp>
#include <string>
#endif

namespace s1
{
  namespace codegen
  {
    CgGenerator::ProgramCodeGenerator::ProgramCodeGenerator (const CgOptions& options)
      : sl::ProgramCodeGenerator (CgTraits::instance, options)
    {
    }

    const CgOptions& CgGenerator::ProgramCodeGenerator::GetCgOptions () const
    {
      return static_cast<const CgOptions&> (options);
    }

    std::unique_ptr<sl::FunctionCodeGenerator> CgGenerator::ProgramCodeGenerator::CreateFunctionCodeGenerator ()
    {
      return std::unique_ptr<sl::FunctionCodeGenerator> (new FunctionCodeGenerator (GetCgOptions()));
    }

  } // namespace codegen
} // namespace s1

